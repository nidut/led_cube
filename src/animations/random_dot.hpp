#ifndef RANDOMDOT_H
#define RANDOMDOT_H

#include "Arduino.h"
#include "ledcube.hpp"

static int randomDot_last_run = 0;
static const int RANDOMDOT_FREQ = 5; // [Hz]

void randomDot(LedCube& cube);

#endif