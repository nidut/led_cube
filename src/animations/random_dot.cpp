#include "random_dot.hpp"

using namespace RandomDot;

static uint32_t last_run = 0;

void RandomDot::draw(LedCube &cube, uint8_t freq)
{
  if (millis() - last_run > 1000 / freq)
  {
    cube.setCube(false);
    delay(1);
    cube.setVoxel(random(cube.getSizeX()), random(cube.getSizeY()), random(cube.getSizeZ()));
    last_run = millis();
  }
}