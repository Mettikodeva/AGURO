#include "aguro.h"
#include "sensor.h"
#include "definitions.h"
Aguro aguro;
Sensor mysensor;
void (*resetFunc)(void) = 0;

void setup()
{
    // Serial.begin(115200);
    FastLED.addLeds<LED_TYPE, LED_PIN, COLOR_ORDER>(leds, NUM_LEDS);
    FastLED.setBrightness(BRIGHTNESS);
    aguro.init(true, &mysensor);
}

bool run = false;
int pb_counter = 0;
void reset()
{
    blink_led(5, 100, CRGB::Yellow);
    aguro.stop_motor();
    resetFunc();
}

void loop()
{
    if (digitalRead(PB) == HIGH)
    {
        run = !run;
        pb_counter++;
        if (pb_counter > 200)
            reset();
    }
    else
    {
        aguro.updateSensor();
        if (pb_counter > 100)
        { // stop the program
            aguro.stop();
        }
    }
    aguro.stop_motor();
    blink_led(1, 100, CRGB::White);
    while (aguro.isStarted())
    {
        if (digitalRead(PB) == HIGH)
        {
            run = !run;
            pb_counter++;
            if (pb_counter > 200)
                reset();
        }
        else
        {
            if (pb_counter > 100)
            { // stop the program
              // aguro.stop();
            }
        }
        if (run)
        {
            aguro.traceLine(180);
        }
        else
            aguro.updateSensor();
    }
}
