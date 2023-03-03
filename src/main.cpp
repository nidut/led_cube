#include "Arduino.h"

#include "ledcube.hpp"
#include "animations/blinking_grid.hpp"
#include "animations/random_dot.hpp"
#include "animations/planboing.hpp"
#include "animations/rain.hpp"
#include "animations/bouncing_ball.hpp"
#include "animations/random_fill.hpp"
#include "animations/ripples.hpp"
#include "animations/fill.hpp"
#include "animations/plane_random_suspend.hpp"
#include "animations/string_fly.hpp"

/* Pins */
const uint8_t LED_INTERNAL = 2;
const uint8_t PIN_SHIFT = 19;
const uint8_t PIN_STORE = 18;
const uint8_t PIN_DATA  = 26;
const uint8_t LAYER_PINS[8] =
{   
    27,
    25,
    32,
    22,
    21,
    17,
    3,  // Pin 3 RXD -> not useable with Serial Connection active
    1   // Pin 1 TXD -> not useable with Serial Connection active
    
};
const uint8_t PIN_POTI = 36;
const uint8_t PIN_BUTTON = 16;

/* Constants */
const int BUTTON_DEBOUNCE = 300; // [ms]
const int POTI_UPDATE_RATE = 10; // [ms]
const uint8_t NUM_OF_ANIMATIONS = 7;
const int REFRESH_RATE = 100; // [Hz] refresh rate for complete cube (all layers)
const uint32_t PWM_FREQ = 125000;
const int PWM_RES = 8;
const int PWM_OFF = pow(2,PWM_RES)-1;

/* TODO */
// animations geschwindigkeit über poti? (zB poti bei gedrückter Taste)
// kaputte LED austauschen
// vcc stabilisieren? (flash Probleme mit desktop PC)


/* Globals */
volatile uint8_t brightness_percent; // non-linear due to LED
// 100% -> 0
// 50% -> 255
// 0% -> 511
hw_timer_t *timer_refresh = NULL;
hw_timer_t *timer_poti = NULL;
LedCube cube;
uint8_t act_z;
uint8_t prev_z;
volatile uint8_t active_animation;
uint32_t last_change_state;
float animation_speed_factor;

void IRAM_ATTR ISR_refreshCube()
{
    // deactivate shift register output    
    digitalWrite(PIN_STORE, LOW);
    // fill shift register with new data
    for (uint8_t y = 0; y < cube.getSizeY(); y++) 
    {
        uint8_t output_byte = 0;
        // here SIZE_X is hardcoded to 8 because of shiftOut expects 1 Byte, 
        // more logic would be needed for variable SIZE_X
        for (uint8_t x = 0; x < 8; x++) 
        {
            output_byte += cube.getVoxel(x, y, act_z) << x;
        }
        shiftOut(PIN_DATA, PIN_SHIFT, LSBFIRST, ~output_byte);
    }
    //switch off previous layer  
    ledcWrite(prev_z, PWM_OFF);
    // switch on new layer
    int pwm_on = PWM_OFF - (PWM_OFF*brightness_percent/100);
    ledcWrite(act_z, pwm_on);
    // activate shift register output
    digitalWrite(PIN_STORE, HIGH);
    // increment counter
    prev_z = act_z;    
    act_z++;
    if (act_z >= cube.getSizeZ())
    {
        act_z = 0;
    }
}
void IRAM_ATTR ISR_getPotiValue()
{
    int analog_value = analogRead(PIN_POTI);
    if (digitalRead(PIN_BUTTON)) 
    {
        // button not pressed
        animation_speed_factor = map(analog_value, 0, 4095, 20, 5)/10;
    }
    else 
    {
        // button pressed
        //brightness_percent = map(analog_value, 0, 4095, 5, 100);
    }
}
void IRAM_ATTR ISR_changeState() 
{
    if (millis() - last_change_state > BUTTON_DEBOUNCE) 
    {
        last_change_state = millis();
        active_animation++;
        if (active_animation == NUM_OF_ANIMATIONS) 
        {
        active_animation = 0;    
        }
    }
}
void setupPins()
{
    pinMode(LED_INTERNAL, OUTPUT);
    pinMode(PIN_STORE, OUTPUT);
    pinMode(PIN_SHIFT, OUTPUT);
    pinMode(PIN_DATA, OUTPUT);
    for (int z = 0; z < 8; z++) 
    {
        ledcAttachPin(LAYER_PINS[z], z);
        ledcSetup(z, PWM_FREQ, PWM_RES);
    }
    pinMode(PIN_BUTTON, INPUT_PULLUP);
}
void setupTimerRefreshCube()
{
    timer_refresh = timerBegin(0, 80, true);
    timerAttachInterrupt(timer_refresh, &ISR_refreshCube, true);
    // base clock: 80 MHz
    // prescaler: 80
    // timer tick: 1µs
    timerAlarmWrite(timer_refresh, 1000000/(8*REFRESH_RATE), true);
    timerAlarmEnable(timer_refresh);
}
void setupTimerPoti()
{
    timer_poti = timerBegin(2, 80, true);
    timerAttachInterrupt(timer_poti, &ISR_getPotiValue, true);
    // base clock: 80 MHz
    // prescaler: 80
    // timer tick: 1µs
    timerAlarmWrite(timer_poti, POTI_UPDATE_RATE*1000, true);
    timerAlarmEnable(timer_poti);
}
void setup()
{
    setupPins();
    setupTimerRefreshCube();
    setupTimerPoti();
    attachInterrupt(PIN_BUTTON, ISR_changeState, FALLING);
    //Serial.begin(115200);
    act_z = 1;
    prev_z = 0;
    active_animation = 5;
    brightness_percent = 25;
    last_change_state = 0;
    animation_speed_factor = 1;
    digitalWrite(LED_INTERNAL, HIGH);
}

void loop()
{
    switch (active_animation)
    {
        case 0:
            PlaneRandSuspend::draw(cube, 2, 15);
            break;
        case 1:
            Rain::draw(cube, 10);
            break;
        case 2:
            Planboing::draw(cube, 2, 10);
            break;
        case 3:
            RandomFill::draw(cube, 20);
            break;
        case 4:
            Ripples::draw(cube, 500);
            break;
        case 5:
            StringFly::draw(cube, "Nico", 10);
            break;
        case 6:
            BouncingBall::draw(cube);
            break;
        default:
            BlinkingGrid::draw(cube);
            break;
    }
}