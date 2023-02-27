#ifndef PLANBOING_H
#define PLANBOING_H

#include "ledcube.hpp"
#include "Arduino.h"


namespace Planboing
{

void draw(LedCube& cube, int plane, int freq = 10);

}
#endif