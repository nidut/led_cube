#include "plane_random_suspend.hpp"

using namespace PlaneRandSuspend;

static uint32_t last_run = 0;
uint8_t positions[64];
uint8_t destinations[64];
uint8_t iterations = 0;
static bool direction = true;

void PlaneRandSuspend::draw(LedCube &cube, uint8_t axis, int freq)
{
    // start of animation
    if (iterations == 0)
    {
        cube.setCube(false);
        cube.setPlane(axis, direction ? 0 : 7);
        for (int i = 0; i < 64; i++)
        {
            positions[i] = direction ? 0 : 7;
            destinations[i] = random(1, 7);
        }
        iterations++;
        delay(1000);
    }

    // animation step
    if (millis() - last_run > 1000 / freq)
    {
        last_run = millis();

        // destination reached
        if (iterations > 7 && iterations < 10)
        {
            for (int i = 0; i < 64; i++)
            {
                destinations[i] = direction ? 7 : 0;
            }
            iterations++;
            return;
        }

        // move voxel
        for (int px = 0; px < 64; px++)
        {
            if (direction && positions[px] < destinations[px])
            {
                positions[px]++;
            }
            if (!direction && positions[px] > destinations[px])
            {
                positions[px]--;
            }
        }

        // draw new positions
        cube.setCube(false);
        for (int a = 0; a < 8; a++)
        {
            for (int b = 0; b < 8; b++)
            {
                uint8_t c = positions[(a * 8) + b];
                switch (axis)
                {
                case 0:
                    cube.setVoxel(c, b, a);
                    break;
                case 1:
                    cube.setVoxel(a, c, b);
                    break;
                case 2:
                    cube.setVoxel(a, b, c);
                    break;
                default:
                    cube.setVoxel(c, b, a);
                    break;
                }
            }
        }

        iterations++;

        // animation finished, restart in opposite direction
        if (iterations == 16)
        {
            iterations = 0;
            direction = !direction;
        }
    }
}