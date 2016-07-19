#include "Eye.h"
#include <cmath>

Eye::Eye()
{
}


Eye::~Eye()
{
}

std::vector<double> Eye::getViewInputs(const BWAPI::Unit& unit)
{
    auto visibleUnits = unit->getUnitsInRadius(viewRadius);
    auto selfPosition = unit->getPosition();
    auto selfPlayer = unit->getPlayer();

    double radiusPerSegment = viewRadius / (double)viewRadiusSegments;
    double anglePerSegment = 360 / (double)viewAngleSegments;

    int inputCount = viewRadiusSegments * viewAngleSegments * 2;

    auto inputs = std::vector<double>(inputCount);
    int segmentId = 0;

    // Iterate over all segments
    for (int radiusSegment = 0; radiusSegment < viewRadiusSegments; radiusSegment++)
    {
       for (int angleSegment = 0; angleSegment < viewAngleSegments; angleSegment++)
       {
          // Segment boundaries
          double minRadius = radiusSegment * radiusPerSegment;
          double maxRadius = (1 + radiusSegment) * radiusPerSegment;

          double minAngle = angleSegment * anglePerSegment;
          double maxAngle = (1 + angleSegment) * anglePerSegment;

          int enemyCount = 0;
          int friendCount = 0;

          // Find all units inside the boundaries
          for (auto u : visibleUnits)
          {
             auto position = u->getPosition();
             double radius = selfPosition.getDistance(position);
             double angle = atan2(position.x - selfPosition.x, position.y - selfPosition.y);
             if (radius < maxRadius && radius > minRadius
                && angle < maxAngle && angle > minAngle)
             {
                if (u->getPlayer()->isAlly(selfPlayer))
                {
                   friendCount++;
                }
                else if (u->getPlayer()->isEnemy(selfPlayer))
                {
                   enemyCount++;
                }
             }
          }

          inputs[segmentId * 2 + 0] = enemyCount;
          inputs[segmentId * 2 + 1] = friendCount;

          segmentId++;
       }
    }


    auto pos = unit->getPosition();
    inputs.push_back(pos.x);
    inputs.push_back(pos.y);

    return inputs;
}