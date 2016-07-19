#pragma once

#include <vector>
#include <BWAPI.h>


class Eye
{
public:
   static const int viewRadius = 4;
   static const int viewRadiusSegments = 2;
   static const int viewAngleSegments = 8;

   Eye();
   ~Eye();

   static std::vector<double> getViewInputs(const BWAPI::Unit& unit);
};

