#ifndef BLINKINGGRID_H
#define BLINKINGGRID_H

#include "Arduino.h"
#include "ledcube.hpp"

namespace BlinkingGrid
{

const uint8_t ball_radius = 2;
void draw(LedCube& cube, uint8_t freq = 5);

}

#endif