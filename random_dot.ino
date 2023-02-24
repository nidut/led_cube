void randomDot() {
  if (millis() - randomDot_last_run > 1000/RANDOMDOT_FREQ) {
      setCube(false);
      delay(1);
      setLED(random(SIZE_X), random(SIZE_Y), random(SIZE_Z));
      randomDot_last_run = millis();
  }
}