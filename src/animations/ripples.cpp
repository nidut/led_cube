#include "ripples.hpp"
#include <math.h>

using namespace Ripples;

static int last_run = 0;
static int iterations = 0;
const static int MAX_ITERATION = 100;

void Ripples::draw(LedCube& cube, int freq)
{
    // clear cube if first run
    if (millis() - last_run > 10000/freq)
    {  
        cube.setCube(false);
    }
    if (millis() - last_run > 1000/freq)
    {
        float distance, height;
        float ripple_interval = 1.3;
        cube.setCube(false);
        for (uint8_t x = 0; x < cube.getSizeX(); x++)
        {
            for (uint8_t y = 0; y < cube.getSizeY(); y++)
            {
                distance = distance2d((cube.getSizeX()-1)/2,(cube.getSizeY()-1)/2, x, y);///9.899495*8;
                height = cube.getSizeZ()/2 + sin(distance/ripple_interval + (float) iterations/100) * cube.getSizeZ()/2;
                cube.setVoxel(x, y, (int) height);
            }
        }
        iterations++;
        last_run = millis();
    }
}

float Ripples::distance2d (float x1, float y1, float x2, float y2)
{	
	float dist;
	dist = sqrt((x1-x2)*(x1-x2) + (y1-y2)*(y1-y2));

	return dist;
}