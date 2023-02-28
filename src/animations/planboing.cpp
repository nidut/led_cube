#include "planboing.hpp"

using namespace Planboing;

static int last_run = 0; 
static int i = 0;
static int increment = 1;

// Draw a plane on one axis and send it back and forth.
void Planboing::draw(LedCube& cube, int axis, int freq)
{
	if (millis() - last_run > 1000/freq)
	{
		cube.setCube(false);
		int i_max = 0;
		cube.setPlane(axis, i);
		i_max = cube.getSize(axis);
		if (i == i_max - 1)
		{
			increment = -1;
		}
		if (i == 0) {
			increment = 1;
		}
		i += increment;

		last_run = millis();
	}
}


