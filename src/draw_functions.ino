void setVoxel(byte x, byte y, byte z, bool led_state=true) {
    if (led_state) {
        bitSet(cube[y][z], x);
    }
    else {
        bitClear(cube[y][z], x);
    }
}
void setRow(byte y, byte z, bool led_state=true) {
    if (led_state) {
        cube[y][z] = (1 << SIZE_X) - 1;
    }
    else {
        cube[y][z] = 0;
    }
}
void setPlaneXY(byte z, bool led_state=true) {
    // plane between X and Y axis, along Z axis
    for (byte y = 0; y < SIZE_Y; y++) {
        setRow(y, z, led_state);
    }
}
void setPlaneYZ(byte x, bool led_state=true) {
    // plane between Y and Z axis, along X axis
    for (byte z = 0; z < SIZE_Z; z++) {
        for (byte y = 0; y < SIZE_Y; y++) {
            setVoxel(x, y, z, led_state);
        }
    }
}
void setPlaneXZ(byte y, bool led_state=true) {
    // plane between X and Z axis, along Y axis
    for (byte z = 0; z < SIZE_Z; z++) {
        setRow(y, z, led_state);
    }
}
void setCube(bool led_state)
{
    for (byte z = 0; z < 8; z++) {
        setPlaneXY(z, led_state);
    }
}