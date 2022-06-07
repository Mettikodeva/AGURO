#include "aguro.h"
// #include "protothreads.h"

Aguro aguro;
Sensor mysensor;

static void shortPress()
{
    if (aguro.isStarted())
    {
        aguro.stop();
    }
    else
    {
        aguro._start();
    }
}
static void longPress()
{
    mysensor.calibrateLine();
}

void setup()
{
    Serial.begin(115200);
    myPB.attachLongPress(longPress);
    myPB.attachShortPress(shortPress);
    myPB.start();
    FastLED.addLeds<LED_TYPE, LED_PIN, COLOR_ORDER>(leds, NUM_LEDS);
    FastLED.setBrightness(BRIGHTNESS);
    aguro.init(true, &mysensor);
}

void loop()
{
}
