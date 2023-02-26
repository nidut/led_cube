const byte LED_INTERNAL = 2;

// cube dimensions
const byte SIZE_X = 8;
const byte SIZE_Y = 4;
const byte SIZE_Z = 8;

// pin numbers
const byte PIN_SHIFT = 19;
const byte PIN_STORE = 18;
const byte PIN_DATA  = 26;
const byte LAYER_PINS[SIZE_Z] = {   
    27,
    25,
    32,
    22,
    21,
    17,
    3,  // Pin 3 RXD -> not useable with Serial Connection active
    1   // Pin 1 TXD -> not useable with Serial Connection active
    
};
const byte PIN_POTI = 36;
const byte PIN_BUTTON = 16;

const byte NUM_OF_ANIMATIONS = 3;
const int BUTTON_DEBOUNCE = 500; // [ms]

// refresh rate for complete cube (all layers)
const word REFRESH_RATE = 100; // [Hz]
const word POTI_UPDATE_RATE = 10; // [ms]
const int PWM_FREQ = 125000;
const int PWM_RES = 8;
const int PWM_OFF = pow(2,PWM_RES)-1;

volatile int brightness_percent; // non-linear due to LED
// 100% -> 0
// 50% -> 255
// 0% -> 511


// TODO
// 4x shift register platine löten
// gesamten cube inbetriebnehmen (SIZE_Y = 8)
// animations geschwindigkeit über poti? (zB poti bei gedrückter Taste)
// kaputte LED austauschen

void setCube(bool led_state=true);

// global variables
hw_timer_t *Timer0_Cfg = NULL;
hw_timer_t *Timer2_Cfg = NULL;
byte cube[SIZE_Y][SIZE_Z];
// [Y][Z]
// one byte is one row in x direction
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
byte counter_z;
byte prev_z;
volatile byte active_animation;
int last_change_state;
int randomDot_last_run;
int blinkingGrid_last_run;
bool blinkingGrid_toggle;
float animation_speed_factor;
const int RANDOMDOT_FREQ = 5; // [Hz]
const int BLINKINGGRID_FREQ = 5; // [Hz]

void IRAM_ATTR ISR_refreshCube(){
    // deactivate shift register output    
    digitalWrite(PIN_STORE, LOW);
    // fill shift register with new data
    for (byte y = 0; y < SIZE_Y; y++) {
        shiftOut(PIN_DATA, PIN_SHIFT, LSBFIRST, ~cube[y][counter_z]);
    }
    //switch off previous layer  
    ledcWrite(prev_z, PWM_OFF);
    // switch on new layer
    int pwm_on = PWM_OFF - (PWM_OFF*brightness_percent/100);
    ledcWrite(counter_z, pwm_on);
    // activate shift register output
    digitalWrite(PIN_STORE, HIGH);
    // increment counter
    prev_z = counter_z;    
    counter_z++;
    if (counter_z >= SIZE_Z){
        counter_z = 0;
    }
}

void IRAM_ATTR ISR_changeState() {
    if (millis() - last_change_state > BUTTON_DEBOUNCE) {
      active_animation++;
      if (active_animation >= NUM_OF_ANIMATIONS) {
          active_animation = 0;    
      }
      last_change_state = millis();
    }
}

void IRAM_ATTR ISR_getPotiValue() {
    int analog_value = analogRead(PIN_POTI);
    if (digitalRead(PIN_BUTTON)) {
        // button not pressed
        animation_speed_factor = map(analog_value, 0, 4095, 20, 5)/10;
    }
    else {
        // button pressed
        brightness_percent = map(analog_value, 0, 4095, 0, 100);
    }
}

void setupPins() {
    pinMode(LED_INTERNAL, OUTPUT);
    pinMode(PIN_STORE, OUTPUT);
    pinMode(PIN_SHIFT, OUTPUT);
    pinMode(PIN_DATA, OUTPUT);
    for (int z = 0; z < SIZE_Z; z++) {
        ledcAttachPin(LAYER_PINS[z], z);
        ledcSetup(z, PWM_FREQ, PWM_RES);
    }
    pinMode(PIN_BUTTON, INPUT_PULLUP);
}

void setupTimerRefreshCube(){
    Timer0_Cfg = timerBegin(0, 80, true);
    timerAttachInterrupt(Timer0_Cfg, &ISR_refreshCube, true);
    // base clock: 80 MHz
    // prescaler: 80
    // timer tick: 1µs
    timerAlarmWrite(Timer0_Cfg, 1000000/(SIZE_Z*REFRESH_RATE), true);
    timerAlarmEnable(Timer0_Cfg);
}
void setupTimerBrightness() {
    Timer2_Cfg = timerBegin(2, 80, true);
    timerAttachInterrupt(Timer2_Cfg, &ISR_getPotiValue, true);
    // base clock: 80 MHz
    // prescaler: 80
    // timer tick: 1µs
    timerAlarmWrite(Timer2_Cfg, POTI_UPDATE_RATE*1000, true);
    timerAlarmEnable(Timer2_Cfg);
}

void setup() {
    setupPins();
    setupTimerRefreshCube();
    setupTimerBrightness();
    attachInterrupt(PIN_BUTTON, ISR_changeState, FALLING);
    //Serial.begin(115200);
    counter_z = 1;
    prev_z = 0;
    active_animation = 0;
    brightness_percent = 25;
    last_change_state = 0;
    randomDot_last_run = 0;
    blinkingGrid_last_run = 0;
    blinkingGrid_toggle = true;
    animation_speed_factor = 1;
    digitalWrite(LED_INTERNAL, HIGH);
}

void loop() {
    switch (active_animation) {
        case 0:
            randomDot();
            break;
        case 1:
            blinkingGrid();
            break;
        case 2:
            setCube();
            break;
    }
}