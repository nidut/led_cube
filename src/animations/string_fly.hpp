#ifndef STRINGFLY_H
#define STRINGFLY_H

#include "ledcube.hpp"
#include "font.hpp"
#include <string>

using namespace std;

namespace StringFly
{

    void draw(LedCube &cube, string text, uint8_t freq = 5);

}

#endif