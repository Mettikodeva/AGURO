#include "aguro.h"
#include "sensor.h"
#include "definitions.h"
// #include "protothreads.h"

Aguro aguro;
Sensor mysensor;

// static void shortPress()
// {
//     if (aguro.isStarted())
//     {
//         aguro.stop();
//     }
//     else
//     {
//         aguro._start();
//     }
// }
// static void longPress()
// {
//     mysensor.calibrateLine();
// }

void setup()
{
    Serial.begin(115200);
    // myPB.attachLongPress(longPress);
    // myPB.attachShortPress(shortPress);
    // myPB.start();
    FastLED.addLeds<LED_TYPE, LED_PIN, COLOR_ORDER>(leds, NUM_LEDS);
    FastLED.setBrightness(BRIGHTNESS);
    Serial.println("Setup");
    aguro.init(true, &mysensor);
    Serial.println("Setup finish");
}

void loop()
{
    aguro.updateSensor();
    aguro.followUntil(TJ, 150);
    delay(1000);
    aguro.followUntil(TJ, 150);
    delay(1000);
    aguro.followUntil(TJ, 150);
    delay(1000);
    aguro.followUntil(TJ, 150);
    delay(1000);
    aguro.followUntil(TJ, 150);
    delay(1000);
    aguro.followUntil(TJ, 150);
    aguro.left(100,100);
    aguro.followUntil(FL, 150);
    // aguro.traceLine(130);
    // aguro.centering();
    // aguro.right(120, 200);
    // for (int i = 0; i < 3; i++)
    //     aguro.followUntil(TJ, 120);
    // aguro.followUntil(FL, 120);
    // aguro.left(120, 200);
    // aguro.backward(120, 200);
}
