#include <FastLED.h>

#define LED_PIN 12
#define LED_TYPE WS2812B
#define NUM_LEDS 8
#define COLOR_ORDER RGB
#define BRIGHTNESS 20
CRGB leds[NUM_LEDS];
#define UPDATES_PER_SECOND 100

void setup()
{
    FastLED.addLeds<LED_TYPE, LED_PIN, COLOR_ORDER>(leds, NUM_LEDS);
    FastLED.setBrightness(BRIGHTNESS);
}
int shift = 0;
void loop()
{
    for (int j = 0; j < 80; j++)
    {
        for (int i = 0; i < NUM_LEDS; i++)
        {
            if (i <= shift)
            {
                leds[i] = CRGB(255, 255, 0);
            }
            else
            {
                leds[i] = CRGB(0, 0, 0);
            }
            delay(10);
        }
        FastLED.show();
        shift++;
        if (shift >= NUM_LEDS)
        {
            shift = 0;
        }
    }
}