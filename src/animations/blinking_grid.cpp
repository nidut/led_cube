#include "blinking_grid.hpp"

void blinkingGrid(LedCube& cube)
{
    if (millis() - blinkingGrid_last_run > 1000/BLINKINGGRID_FREQ) {
        blinkingGrid_toggle = !blinkingGrid_toggle;
        blinkingGrid_last_run = millis();    
    }
    for (byte z = 0; z < cube.getSizeZ(); z++) {
        for (byte y = 0; y < cube.getSizeY(); y++) {
            for (byte x = 0; x < cube.getSizeX(); x++) {
                if ((x+y+z) % 2 ==  blinkingGrid_toggle) {
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