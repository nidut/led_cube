#include "random_fill.hpp"

using namespace RandomFill;

static int last_run = 0;

void RandomFill::draw(LedCube& cube, int freq, bool state)
{
    // clear cube if first run
    if (millis() - last_run > 10000/freq)
    {  
        cube.setCube(!state);
    }
    if (millis() - last_run > 1000/freq)
    {
        uint8_t x = random(cube.getSizeX());
        uint8_t y = random(cube.getSizeY());
        uint8_t z = random(cube.getSizeZ());
        if (cube.getVoxel(x, y, z) != state)
        {
            cube.setVoxel(x, y, z, state);
            last_run = millis();
        }
    }
}