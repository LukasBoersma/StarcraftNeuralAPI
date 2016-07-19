#include "AIManager.h"

#include <iostream>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <iphlpapi.h>
#include <sstream>

#include <BWAPI.h>


#define API_PORT 11421

using namespace BWAPI;

#pragma comment(lib, "Ws2_32.lib")

AIManager::AIManager()
{

}

AIManager::~AIManager()
{
}

void AIManager::openSocket()
{
   WSAData version;        //We need to check the version.
   int ok = WSAStartup(MAKEWORD(2, 2), &version);
   if (ok != NO_ERROR)
   {
      throw std::exception("Unable to initialize API winsock");
   }

   struct addrinfo *result = NULL, *ptr = NULL, hints;
   
   apiSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
   
   if (apiSocket == INVALID_SOCKET)
   {
      freeaddrinfo(result);
      WSACleanup();
      throw std::exception("Unable to initialize API socket");
   }

   int timeout = 2000;

   setsockopt(apiSocket, SOL_SOCKET, SO_SNDTIMEO, (const char*)&timeout, sizeof(int));
   setsockopt(apiSocket, SOL_SOCKET, SO_RCVTIMEO, (const char*)&timeout, sizeof(int));

   sockaddr_in clientService;
   clientService.sin_family = AF_INET;
   clientService.sin_addr.s_addr = inet_addr("192.168.1.149");
   clientService.sin_port = htons(API_PORT);

   ok = connect(apiSocket, (SOCKADDR*)&clientService, sizeof(clientService));
   
   if (ok == SOCKET_ERROR) {
      int error = WSAGetLastError();
      closesocket(apiSocket);
      WSACleanup();
      std::stringstream s;
      s << "Unable to connect API socket, code " << error;
      throw std::exception(s.str().c_str());
   }

   isConnected = true;
}

void AIManager::close()
{
   // shutdown the send half of the connection since no more data will be sent
   int ok = shutdown(apiSocket, SD_SEND);
   if (ok == SOCKET_ERROR) {
      throw std::exception("Socket shutdown failed");
   }
   closesocket(apiSocket);
   WSACleanup();
}

std::vector<double> AIManager::getBrainOutputs(std::vector<double> inputs)
{
   int dataLen = inputs.size() * sizeof(double);
   int headSize = 8;
   int totalLen = dataLen + headSize;
   
   char* buffer = new char[totalLen];

   ZeroMemory(buffer, totalLen);

   buffer[0] = 37;
   buffer[1] = dataLen / 256;
   buffer[2] = dataLen % 256;

   memcpy(buffer + headSize, inputs.data(), dataLen);

   int ok = send(apiSocket, buffer, totalLen, 0);
   std::cout << "Sent " << totalLen << " bytes";

   delete[] buffer;

   if (ok == SOCKET_ERROR)
   {
      int error = WSAGetLastError();
      closesocket(apiSocket);
      WSACleanup();
      std::stringstream s;
      s << "Socket error while sending, code " << error;
      throw std::exception(s.str().c_str());
   }

   std::vector<char> outputBuffer;

   int receiveLen = 1024;
   char* receiveBuffer = new char[receiveLen];

   auto rcvLastSeen = GetTickCount64();

   while ((int)outputBuffer.size() < headSize)
   {
      int readBytes;
      receiveSome(receiveBuffer, readBytes, receiveLen);

      auto now = GetTickCount64();

      if (readBytes <= 0 && now - rcvLastSeen > 2000)
         throw std::exception("Receive timeout");
      else
         rcvLastSeen = now;

      for (int i = 0; i < readBytes; i++)
         outputBuffer.push_back(receiveBuffer[i]);
   }
   
   int receivedDataLen = outputBuffer[1] * 256 + outputBuffer[2];
   int receivedTotalLen = headSize + receivedDataLen;

   if (receivedDataLen % sizeof(double) != 0)
      throw std::exception("Invalid data length, must be multiple of sizeof(double)");

   rcvLastSeen = GetTickCount64();

   while ((int)outputBuffer.size() < receivedTotalLen)
   {
      int readBytes;
      receiveSome(receiveBuffer, readBytes, receiveLen);

      auto now = GetTickCount64();

      if (readBytes <= 0 && now - rcvLastSeen > 2000)
         throw std::exception("Receive timeout");
      else
         rcvLastSeen = now;

      for (int i = 0; i < readBytes; i++)
         outputBuffer.push_back(receiveBuffer[i]);
   }

   std::cout << "Received " << receivedTotalLen << " bytes";

   int outputCount = receivedTotalLen / sizeof(double);

   double* outputDataBuffer = new double[outputCount];

   memcpy(outputDataBuffer, outputBuffer.data() + headSize, receivedDataLen);

   std::vector<double> output;
   output.resize(outputCount);
   
   for (int i = 0; i < outputCount; i++)
   {
      output[i] = outputDataBuffer[i];
   }
   
   delete[] receiveBuffer;
   delete[] outputDataBuffer;

   return output;
}

void AIManager::receiveSome(char* buffer, int& readLength, int maxLength)
{
   int receivedBytes = recv(apiSocket, buffer, maxLength, 0);
   if (receivedBytes < 0)
   {
      closesocket(apiSocket);
      WSACleanup();
      throw std::exception("Socket error while receiving");
   }

   readLength = receivedBytes;
}