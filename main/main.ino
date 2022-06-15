#include "aguro.h"
#include "sensor.h"
#include "definitions.h"

Aguro aguro;
Sensor mysensor;

bool start_state = true;
void myInterrupt(){
    start_state = !start_state;
    while(start_state);
}


void setup()
{
    // Serial.begin(115200);
    pinMode(3, INPUT_PULLUP);
    delayMicroseconds(100);
    attachInterrupt(digitalPinToInterrupt(3),myInterrupt,FALLING);
    FastLED.addLeds<LED_TYPE, LED_PIN, COLOR_ORDER>(leds, NUM_LEDS);
    FastLED.setBrightness(BRIGHTNESS);
    Serial.println("Setup");
    aguro.init(true, &mysensor);
    Serial.println("Setup finish");
    aguro._start();
    aguro.magnet(false);
}

void loop()
{
    // aguro.traceLine(100);

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
    aguro.mundur(90,2200);
    delay(1000);
    // delay(200);
    // aguro.followUntil(FL, speed);

    // delay(5000);
    // aguro.right(150,360); // ~90 degree
    // delay(2000);
    // delay(5000);
    // aguro.followUntil(FR, speed);
    // delay(5000);
    // aguro.followUntil(FL, speed);
    // delay(2000);
    // aguro.followUntil(FL, speed);
    // aguro.left(155,340);
    // delay(2000);

    // delay(1000);
    // delay(10000);

    // aguro.followUntil(FL, 115);
    // aguro.followUntil(FL, 115);
    // aguro.followUntil(FL, 115);
    // for (int i = 0; i < 3; i++)
    //     aguro.followUntil(TJ, 120);
    // aguro.followUntil(FL, 120);
}
