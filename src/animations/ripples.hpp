#ifndef RIPPLES_H
#define RIPPLES_H

#include "ledcube.hpp"

namespace Ripples
{

    void draw(LedCube &cube, int freq = 10);
    float distance2d(float x1, float y1, float x2, float y2);

}
#endif