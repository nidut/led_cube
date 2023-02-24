void blinkingGrid() {
    if (millis() - blinkingGrid_last_run > 1000/BLINKINGGRID_FREQ) {
        blinkingGrid_toggle = !blinkingGrid_toggle;
        blinkingGrid_last_run = millis();    
    }
    for (byte z = 0; z < SIZE_Z; z++) {
        for (byte y = 0; y < SIZE_Y; y++) {
            for (byte x = 0; x < SIZE_X; x++) {
                if ((x+y+z) % 2 ==  blinkingGrid_toggle) {
                    bitSet(cube[y][z], x);  
                }
                else {
                    bitClear(cube[y][z], x);
                }
            }
        }
    }
}