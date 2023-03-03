#include "string_fly.hpp"

using namespace StringFly;
using namespace std;

static uint32_t last_run = 0;
static uint8_t text_position = 0;
static uint8_t iteration = 0;
static unsigned char chr[5];

void StringFly::draw(LedCube &cube, string text, uint8_t freq)
{
    if (millis() - last_run > 10000 / freq)
    {
        cube.setCube(false);
    }
    if (millis() - last_run > 1000 / freq)
    {
        last_run = millis();

        // repeat text from start
        if (text_position == text.length())
        {
            text_position = 0;
        }

        // shift cube from back to front 1 step
        cube.shift(1, -1);

        // draw next letter
        if (iteration == 0)
        {
            font_getchar(text[text_position], chr);
            for (int x = 0; x < 5; x++)
            {
                for (int y = 0; y < 8; y++)
                {
                    if ((chr[x] & (0x80 >> y)))
                    {
                        cube.setVoxel(x + 2, 7, y);
                    }
                }
            }
        }

        iteration++;

        // letter reached front, switch to next letter
        if (iteration == 8)
        {
            text_position++;
            iteration = 0;
        }
    }
}