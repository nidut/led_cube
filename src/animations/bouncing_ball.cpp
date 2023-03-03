#include "bouncing_ball.hpp"

using namespace BouncingBall;

static uint32_t  last_run = 0;
static std::array<int8_t, 3> position = {0, 0, 0};
static std::array<int8_t, 3> move_vector = {0, 0, 0};

void BouncingBall::draw(LedCube& cube, int freq)
{
    if (millis() - last_run > 10000/freq || last_run == 0)
    {
        //first run
        position = { 
            random(cube.getSizeX()), 
            random(cube.getSizeY()), 
            random(cube.getSizeZ())
        };
        move_vector = {
            random(1) ? 1 : -1,
            random(1) ? 1 : -1,
            random(1) ? 1 : -1
        };
    }
    if (millis() - last_run > 1000/freq) {
        cube.setCube(false);
        static std::array<int8_t, 3> new_position;
        for (int axis = 0; axis < 3; axis++)
        {
            new_position[axis] = position[axis] + move_vector[axis];
            if (new_position[axis] >= cube.getSize(axis))
            {
                int outside_distance = new_position[axis] - (cube.getSize(axis) - 1);
                new_position[axis] = (cube.getSize(axis) - 1) - outside_distance;
                move_vector[axis] = -1 * move_vector[axis];
            }
            else if(new_position[axis] < 0)
            {
                new_position[axis] = -1* new_position[axis];
                move_vector[axis] = -1 * move_vector[axis];
            }
        }
        position = new_position;
        cube.setVoxel(position[0], position[1], position[2]);
        last_run = millis();
    }
}