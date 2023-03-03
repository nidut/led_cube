#ifndef RANDOMDOT_H
#define RANDOMDOT_H

#include "Arduino.h"
#include "ledcube.hpp"

namespace RandomDot
{

    void draw(LedCube &cube, uint8_t freq = 5);

}

#endif