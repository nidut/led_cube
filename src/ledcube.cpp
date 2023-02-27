#include "ledcube.hpp"

void LedCube::setVoxel(uint8_t x, uint8_t y, uint8_t z, bool led_state) {
    cube[x][y][z] = led_state;
}
bool LedCube::getVoxel(uint8_t x, uint8_t y, uint8_t z) {
    return cube[x][y][z];
}
void LedCube::setRow(uint8_t y, uint8_t z, bool led_state) {
    for (uint8_t i = 0; i < SIZE_X; i++) {
        cube[i][y][z] = led_state;
    }
}
void LedCube::setPlaneXY(uint8_t z, bool led_state) {
    // plane between X and Y axis, along Z axis
    for (uint8_t y = 0; y < SIZE_Y; y++) {
        setRow(y, z, led_state);
    }
}
void LedCube::setPlaneYZ(uint8_t x, bool led_state) {
    // plane between Y and Z axis, along X axis
    for (uint8_t z = 0; z < SIZE_Z; z++) {
        for (uint8_t y = 0; y < SIZE_Y; y++) {
            setVoxel(x, y, z, led_state);
        }
    }
}
void LedCube::setPlaneXZ(uint8_t y, bool led_state) {
    // plane between X and Z axis, along Y axis
    for (uint8_t z = 0; z < SIZE_Z; z++) {
        setRow(y, z, led_state);
    }
}
void LedCube::setCube(bool led_state)
{
    for (uint8_t z = 0; z < SIZE_Z; z++) {
        setPlaneXY(z, led_state);
    }
}
uint8_t LedCube::getSizeX()
{
    return this->SIZE_X;
}
uint8_t LedCube::getSizeY()
{
    return this->SIZE_Y;
}
uint8_t LedCube::getSizeZ()
{
    return this->SIZE_Z;
}
LedCube::LedCube()
{

}
LedCube::~LedCube()
{
    
}