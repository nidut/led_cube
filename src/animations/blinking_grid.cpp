#include "blinking_grid.hpp"

using namespace BlinkingGrid;

static int last_run = 0; 
static bool toggle = true;

void BlinkingGrid::draw(LedCube& cube, uint8_t freq)
{
    if (millis() - last_run > 1000/freq) {
        toggle = !toggle;
        last_run = millis();    
    }
    for (byte z = 0; z < cube.getSizeZ(); z++) {
        for (byte y = 0; y < cube.getSizeY(); y++) {
            for (byte x = 0; x < cube.getSizeX(); x++) {
                if ((x+y+z) % 2 ==  toggle) {
                    cube.setVoxel(x, y, z);
                    //bitSet(cube[y][z], x);  
                }
                else {
                    cube.setVoxel(x, y, z, false);
                    //bitClear(cube[y][z], x);
                }
            }
        }
    }
}