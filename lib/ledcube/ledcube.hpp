#ifndef LEDCUBE_H
#define LEDCUBE_H

#include "Arduino.h"

// true = LED on, false = LED off
//
//       o o o o o o o o
//     o o o o o o o o o
//   o o o o o o o o o o
// 0 0 0 0 0 0 0 0 o o o
// 0 0 0 0 0 0 0 0 o o o
// 0 0 0 0 0 0 0 0 o o o
// Z 0 0 0 0 0 0 0 o o o
// ^ 0 0 Y 0 0 0 0 o o o
// | 0 / 0 0 0 0 0 o o
// | / 0 0 0 0 0 0 o
// 0 ----->X 0 0 0
//

class LedCube
{
public:
    LedCube(uint8_t base = BOTTOM);
    ~LedCube();
    void setVoxel(uint8_t x, uint8_t y, uint8_t z, bool led_state = true);
    bool getVoxel(uint8_t x, uint8_t y, uint8_t z, bool raw = false);
    void setRow(uint8_t y, uint8_t z, bool led_state = true);
    void setPlane(uint8_t axis, uint8_t position, bool led_state = true);
    void setPlaneXY(uint8_t z, bool led_state = true);
    void setPlaneYZ(uint8_t x, bool led_state = true);
    void setPlaneXZ(uint8_t y, bool led_state = true);
    void setCube(bool led_state = true);
    void shift(uint8_t axis, int8_t distance);
    void rotate(int8_t axis_position_x = 4, int8_t axis_position_y = 3, bool rotation_direction = true);
    uint8_t getSize(uint8_t axis);
    uint8_t getSizeX();
    uint8_t getSizeY();
    uint8_t getSizeZ();
    bool isVoxelInCube(uint8_t x, uint8_t y, uint8_t z);
    void setCubeBase(uint8_t base = BOTTOM);
    uint8_t getCubeBase();
    void transformVoxel(uint8_t & x, uint8_t & y, uint8_t & z, uint8_t cube_base);
    const static uint8_t BOTTOM = 0;
    const static uint8_t TOP = 1;
    const static uint8_t LEFT = 2;
    const static uint8_t RIGHT = 3;
    const static uint8_t FRONT = 4;
    const static uint8_t REAR = 5;
private:
    const static uint8_t SIZE_X = 8;
    const static uint8_t SIZE_Y = 8;
    const static uint8_t SIZE_Z = 8;
    uint8_t cube_base;
    std::array<std::array<std::array<bool, SIZE_X>, SIZE_Y>, SIZE_Z> cube;
};
#endif