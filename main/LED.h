#include <FastLED.h>
#define LED_PIN 12
#define LED_TYPE WS2812B
#define NUM_LEDS 8
#define COLOR_ORDER RGB
#define BRIGHTNESS 50
CRGB leds[NUM_LEDS];
#define UPDATES_PER_SECOND 100