#include "rain.hpp"

using namespace Rain;

static int last_run = 0;

void Rain::draw(LedCube& cube, int freq)
{
    // clear cube if first run
    if (millis() - last_run > 10000/freq)
    {  
        cube.setCube(false);
    }
    if (millis() - last_run > 1000/freq) {

        cube.shift(2,-1);
        for (int i=0; i < rand()%4;i++)
        {
            int rnd_x = rand() % cube.getSizeX();
            int rnd_y = rand() % cube.getSizeY();
            cube.setVoxel(rnd_x,rnd_y,cube.getSizeZ()-1);
        }
        last_run = millis();
    }
}