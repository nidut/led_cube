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
        LedCube();
        ~LedCube();
        void setVoxel(uint8_t x, uint8_t y, uint8_t z, bool led_state = true);
        bool getVoxel(uint8_t x, uint8_t y, uint8_t z);
        void setRow(uint8_t y, uint8_t z, bool led_state=true);
        void setPlane(uint8_t axis, uint8_t position, bool led_state=true);
        void setPlaneXY(uint8_t z, bool led_state=true);
        void setPlaneYZ(uint8_t x, bool led_state=true);
        void setPlaneXZ(uint8_t y, bool led_state=true);
        void setCube(bool led_state=true);
        uint8_t getSize(uint8_t axis);
        uint8_t getSizeX();
        uint8_t getSizeY();
        uint8_t getSizeZ();
    private:
        const static uint8_t SIZE_X = 8;
        const static uint8_t SIZE_Y = 4;
        const static uint8_t SIZE_Z = 8;
        std::array<std::array<std::array<bool, SIZE_X>, SIZE_Y>, SIZE_Z> cube;
};
#endif