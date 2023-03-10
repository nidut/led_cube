#include "ledcube.hpp"

void LedCube::setVoxel(uint8_t x, uint8_t y, uint8_t z, bool led_state)
{
    if (isVoxelInCube(x, y, z))
    {
        // data storage always with cube_base = BOTTOM
        cube[x][y][z] = led_state;
    }
}
bool LedCube::getVoxel(uint8_t x, uint8_t y, uint8_t z, bool raw)
{
    bool value = false;
    if (isVoxelInCube(x, y, z))
    {
        if (!raw) 
        {
            // read data with regards to cube_base
            transformVoxel(x, y, z, cube_base);
        }
        value = cube[x][y][z];
    }
    return value;
}
void LedCube::transformVoxel(uint8_t & x, uint8_t & y, uint8_t & z, uint8_t cube_base)
{  
    uint8_t new_x;
    uint8_t new_y; 
    uint8_t new_z;
    switch (cube_base)
    {
        case BOTTOM:
            new_x = x;
            new_y = y;
            new_z = z;
            break;
        case TOP:
            new_x = getSizeX()-1-x;
            new_y = getSizeY()-1-y;
            new_z = getSizeZ()-1-z;
            break;
        case RIGHT:
            new_x = z;
            new_y = y;
            new_z = getSizeX()-1-x;
            break;
        case LEFT:
            new_x = getSizeZ()-1-z;
            new_y = y;
            new_z = x;
            break;
        case FRONT:
            new_x = x;
            new_y = getSizeZ()-1-z;
            new_z = y;
            break;
        case REAR:
            new_x = getSizeX()-1-x;
            new_y = z;
            new_z = getSizeY()-1-y;
            break;
    }
    x = new_x;
    y = new_y;
    z = new_z;
}
void LedCube::setRow(uint8_t y, uint8_t z, bool led_state)
{
    for (uint8_t i = 0; i < getSizeX(); i++)
    {
        cube[i][y][z] = led_state;
    }
}
void LedCube::setPlane(uint8_t axis, uint8_t position, bool led_state)
{
    switch (axis)
    {
    case 0:
        // plane between Y and Z axis, along X axis
        setPlaneYZ(position, led_state);
        break;
    case 1:
        // plane between X and Z axis, along Y axis
        setPlaneXZ(position, led_state);
        break;
    case 2:
        // plane between X and Y axis, along Z axis
        setPlaneXY(position, led_state);
        break;
    }
}
void LedCube::setPlaneXY(uint8_t z, bool led_state)
{
    // plane between X and Y axis, along Z axis
    for (uint8_t y = 0; y < getSizeY(); y++)
    {
        setRow(y, z, led_state);
    }
}
void LedCube::setPlaneYZ(uint8_t x, bool led_state)
{
    // plane between Y and Z axis, along X axis
    for (uint8_t z = 0; z < getSizeZ(); z++)
    {
        for (uint8_t y = 0; y < getSizeY(); y++)
        {
            setVoxel(x, y, z, led_state);
        }
    }
}
void LedCube::setPlaneXZ(uint8_t y, bool led_state)
{
    // plane between X and Z axis, along Y axis
    for (uint8_t z = 0; z < getSizeZ(); z++)
    {
        setRow(y, z, led_state);
    }
}
void LedCube::setCube(bool led_state)
{
    for (uint8_t z = 0; z < getSizeZ(); z++)
    {
        setPlaneXY(z, led_state);
    }
}
void LedCube::shift(uint8_t axis, int8_t distance)
{
    std::array<std::array<std::array<bool, SIZE_X>, SIZE_Y>, SIZE_Z> new_cube = cube;
    int8_t distance_x = 0;
    int8_t distance_y = 0;
    int8_t distance_z = 0;
    switch (axis)
    {
    case 0:
        // along X axis
        distance_x = distance;
        break;
    case 1:
        // along Y axis
        distance_y = distance;
        break;
    case 2:
        // along Z axis
        distance_z = distance;
        break;
    }
    for (int x = 0; x < getSizeX(); x++)
    {
        if (x + distance_x < 0 || x + distance_x >= getSizeX())
        {
            continue;
        }
        for (int y = 0; y < getSizeY(); y++)
        {
            if (y + distance_y < 0 || y + distance_y >= getSizeY())
            {
                continue;
            }
            for (int z = 0; z < getSizeZ(); z++)
            {
                new_cube[x][y][z] = false;
                if (z + distance_z < 0 || z + distance_z >= getSizeZ())
                {
                    continue;
                }
                new_cube[x + distance_x][y + distance_y][z + distance_z] = getVoxel(x, y, z, true);
            }
        }
    }
    cube = new_cube;
}
void LedCube::rotate(int8_t axis_position_x, int8_t axis_position_y, bool rotation_direction)
{
    LedCube new_cube(cube_base);
    for (int8_t x = 0; x < getSizeX(); x++)
    {
        for (int8_t y = 0; y < getSizeY(); y++)
        {
            for (int8_t z = 0; z < getSizeZ(); z++)
            {
                int8_t new_x, new_y;
                // no change at rotation axis
                if (x == axis_position_x && y == axis_position_y)
                {
                    new_x = x;
                    new_y = y;
                }
                // y plane of rotation axis, move left or right
                else if (x == axis_position_x)
                {
                    int8_t offset_x = rotation_direction ? (y - axis_position_y) : -1 * (y - axis_position_y);
                    new_x = x - offset_x;
                    new_y = y;
                }
                // x plane of rotation axis, move to back or to front
                else if (y == axis_position_y)
                {
                    int8_t offset_y = rotation_direction ? (x - axis_position_x) : -1 * (x - axis_position_x);
                    new_x = x;
                    new_y = y + offset_y;
                }
                // diagonals
                else if (abs(x - axis_position_x) == abs(y - axis_position_y))
                {
                    if ((x < axis_position_x && y < axis_position_y) || (x > axis_position_x && y > axis_position_y))
                    {
                        new_x = rotation_direction ? axis_position_x : x;
                        new_y = rotation_direction ? y : axis_position_y;
                    }
                    else if ((x < axis_position_x && y > axis_position_y) || (x > axis_position_x && y < axis_position_y))
                    {
                        new_x = rotation_direction ? x : axis_position_x;
                        new_y = rotation_direction ? axis_position_y : y;
                    }
                }
                // ignore all other pixels!
                else{
                    continue;
                }
                new_cube.setVoxel(new_x, new_y, z, this->getVoxel(x, y, z, true));
            }
        }
    }
    cube = new_cube.cube;
}
uint8_t LedCube::getSizeX()
{
    return SIZE_X;
}
uint8_t LedCube::getSizeY()
{
    return SIZE_Y;
}
uint8_t LedCube::getSizeZ()
{
    return SIZE_Z;
}
uint8_t LedCube::getSize(uint8_t axis)
{
    switch (axis)
    {
    case 0:
        return this->getSizeX();
        break;
    case 1:
        return this->getSizeY();
        break;
    case 2:
        return this->getSizeZ();
        break;
    default:
        return this->getSizeX();
        break;
    }
}
bool LedCube::isVoxelInCube(uint8_t x, uint8_t y, uint8_t z)
{
    return (x < getSizeX() && x >= 0 && y < getSizeY() && y >= 0 && z < getSizeZ() && z >= 0);
}
void LedCube::setCubeBase(uint8_t base)
{
    if (base >= 0 && base <= 5)
    {
        cube_base = base;
    }
}
uint8_t LedCube::getCubeBase()
{
    return cube_base;
}
LedCube::LedCube(uint8_t base)
{
    setCube(false);
    cube_base = base;
}
LedCube::~LedCube()
{
}