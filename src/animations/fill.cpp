#include "fill.hpp"

using namespace Fill;

static uint32_t  last_run = 0;
static uint32_t  iterations = 0;

void Fill::draw(LedCube& cube, int freq)
{
    // clear cube if first run
    if (millis() - last_run > 10000/freq)
    {  
        cube.setCube(false);
        last_run = millis();
    }
    if (millis() - last_run > 1000/freq)
    {
        if (iterations == 512)
        {
            iterations = 0;
            cube.setCube(false);
        }

        uint8_t pos_z = iterations / 64;
        uint8_t pos_y = iterations % 64 / 8;
        uint8_t pos_x = iterations % 64 % 8;
        cube.setVoxel(pos_x, pos_y, pos_z);
        iterations++;
        last_run = millis();
    }
}