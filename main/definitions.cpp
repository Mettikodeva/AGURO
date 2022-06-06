#include "definitions.h"

extern CRGB leds[NUM_LEDS] = {
    CRGB(0,0,0),
    CRGB(0,0,0),
    CRGB(0,0,0),
    CRGB(0,0,0),
    CRGB(0,0,0),
    CRGB(0,0,0),
    CRGB(0,0,0),
    CRGB(0,0,0),
};

extern void blink_led(int times, int delay_time, CRGB color)
{
    for (int i = 0; i < times * 2; i++)
    {
        if (i % 2 == 0)
            for (int i = 0; i < NUM_LEDS; i++)
            {
                leds[i] = CRGB::Black;
                FastLED.show();
            }
        else
            for (int i = 0; i < NUM_LEDS; i++)
            {
                leds[i] = color;
                FastLED.show();
            }
        delay(100);
    }
}


extern void led_running(int times, int delay_time, CRGB color)
{
    int shift = 0;
    for (int j = 0; j < times * 10; j++)
    {
        for (int i = 0; i < NUM_LEDS; i++)
        {
            if (i <= shift && i > shift - 4)
            {
                leds[i] = color;
                FastLED.show();
            }
            else
            {
                leds[i] = CRGB(0, 0, 0);
                FastLED.show();
            }
            delay(delay_time);
            // FastLED.show();
        }
        shift++;
        if (shift >= NUM_LEDS + 2)
            shift = 0;
    }
}
extern int make_safe(int val)
{
    if (val > 255)
        return 255;
    else if (val < 0)
        return 0;
    else
        return val;
}