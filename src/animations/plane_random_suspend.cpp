#include "plane_random_suspend.hpp"

using namespace PlaneRandSuspend;

static uint32_t  last_run = 0; 
uint8_t positions[64];
uint8_t destinations[64];
uint8_t iterations = 0;
static bool direction = true;


void PlaneRandSuspend::draw(LedCube& cube, int freq)
{
    if (iterations == 0)
    {  
        last_run = millis();
        cube.setCube(false);
        if (direction) {
            cube.setPlaneYZ(0);
        }
        else {
            cube.setPlaneYZ(7);            
        }

        for (int i = 0; i < 64; i++) {
            if (direction) {
                positions[i] = 0;
            }
            else {
                positions[i] = 7;              
            }
            destinations[i] = random(1,7);
        }
        iterations++;
        delay(1000);
    }
    if (millis() - last_run > 1000/freq) {
        last_run = millis();
        if (iterations > 7 && iterations < 10) {
            for (int i=0; i<64; i++)
            {
                if (direction) {
                    destinations[i] = 7;
                }
                else {
                    destinations[i] = 0;                    
                }
            }
            iterations++;
            return;
        }
        for (int px = 0; px < 64; px++) {
            if (direction && positions[px] < destinations[px]) {
                positions[px]++;
            }
            if (!direction && positions[px] > destinations[px]) {
                positions[px]--;                
            }
        }
        cube.setCube(false);
        for (int a = 0; a < 8; a++) {
            for (int b = 0; b < 8; b++) {
                uint8_t x = positions[(a*8)+b];
                cube.setVoxel(x, b, a);
            }
        }
        iterations++;
        if (iterations == 16) {
            iterations = 0;
            direction = !direction;
        }
    }
}