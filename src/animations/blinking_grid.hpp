#ifndef BLINKINGGRID_H
#define BLINKINGGRID_H

#include "Arduino.h"
#include "ledcube.hpp"

static int blinkingGrid_last_run = 0;
static bool blinkingGrid_toggle = true;
static const int BLINKINGGRID_FREQ = 5; // [Hz]

void blinkingGrid(LedCube& cube);

#endif