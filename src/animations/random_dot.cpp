#include "random_dot.hpp"

void randomDot(LedCube& cube)
{
  if (millis() - randomDot_last_run > 1000/RANDOMDOT_FREQ) {
      cube.setCube(false);
      delay(1);
      cube.setVoxel(random(cube.getSizeX()), random(cube.getSizeY()), random(cube.getSizeZ()));
      randomDot_last_run = millis();
  }
}