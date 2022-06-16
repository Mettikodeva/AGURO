#include "aguro.h"
#include "sensor.h"
#include "definitions.h"

Aguro aguro;
Sensor mysensor;

bool start_state = true;
void myInterrupt()
{
    start_state = !start_state;
    while (start_state)
        ;
}

void setup()
{
    // Serial.begin(115200);
    pinMode(3, INPUT_PULLUP);
    delayMicroseconds(100);
    attachInterrupt(digitalPinToInterrupt(3), myInterrupt, FALLING);

    // LED
    FastLED.addLeds<LED_TYPE, LED_PIN, COLOR_ORDER>(leds, NUM_LEDS);
    FastLED.setBrightness(BRIGHTNESS);

    // AGURO
    aguro.init(true, &mysensor);
    aguro._start();
    aguro.magnet(false);
}

void loop()
{

    int speed = 100;
    // aguro.followUntil(TJ, speed);
    // delay(200);
    // aguro.followUntil(TJ, speed);
    // delay(200);
    // aguro.followUntil(TJ, speed);
    // delay(200);
    // aguro.followUntil(TJ, speed);
    // delay(200);
    // aguro.followUntil(TJ, speed);
    // delay(200);
    // aguro.followUntil(TJ, speed);
    // delay(1000);
    // aguro.left(125,400); // ~90 degree
    // // delay(200);
    // delay(200);
    // aguro.magnet(false);
    aguro.mundur(90, 2200);
    delay(1000);
    // aguro.followUntil(FR, speed);
    // delay(5000);
    // aguro.followUntil(FL, speed);
    // delay(2000);
    // aguro.followUntil(FL, speed);
    // aguro.left(155,340);
    // delay(2000);
}
