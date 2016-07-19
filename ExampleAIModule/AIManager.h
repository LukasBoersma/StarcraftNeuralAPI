#pragma once

#include <winsock2.h>
#include <ws2tcpip.h>
#include <iphlpapi.h>

#include <vector>

#include "Brain.h"

class AIManager
{
public:
   AIManager();
   ~AIManager();

   SharedBrain fetchNewBrain();

   SOCKET apiSocket;
   bool isConnected = false;

   void openSocket();

   std::vector<double> getBrainOutputs(std::vector<double> data);

   void receiveSome(char * buffer, int & readLength, int maxLength);
};

