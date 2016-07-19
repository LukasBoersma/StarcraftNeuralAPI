#pragma once

#include <memory>

class Brain
{
public:
   Brain();
   ~Brain();

   void Tick();

};

typedef std::shared_ptr<Brain> SharedBrain;