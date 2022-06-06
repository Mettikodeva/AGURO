#ifndef definitions_h
#define definitions_h
#include <FastLED.h>
#include "Arduino.h"

#define LED_PIN 12
#define LED_TYPE WS2812B
#define NUM_LEDS 8
#define COLOR_ORDER RGB
#define BRIGHTNESS 50
#define UPDATES_PER_SECOND 100
#define LONG_PRESS 3000
#define MEDIUM_PRESS 1000
#define SHORT_PRESS 300
#define RESET_PRESS 5000
#define PB 13 // push button //PCINT0 //should be input pullup
// void (*resetFunc)(void) = 0;

extern CRGB leds[NUM_LEDS];

extern void blink_led(int times, int delay_time, CRGB color);
extern void led_running(int times, int delay_time, CRGB color);
extern int make_safe(int val);
#endif