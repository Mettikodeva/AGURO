#ifndef definitions_h
#define definitions_h
#include <stdlib.h>
#include <stdio.h>
#include <FastLED.h>
#include "Arduino.h"
#include <EEPROM.h>
// LED
#define LED_PIN 12
#define LED_TYPE WS2812B
#define NUM_LEDS 8
#define COLOR_ORDER RGB
#define BRIGHTNESS 20
// PUSH BUTTON
#define UPDATES_PER_SECOND 100
#define LONG_PRESS 3000
#define MEDIUM_PRESS 1000
#define SHORT_PRESS 300
#define RESET_PRESS 5000

// PIN
#define Relay 13
#define LimitSwitch 1 // perlu di ubah keknya
#define PushButton 3  // push button //PCINT0 //should be input pullup

extern CRGB leds[NUM_LEDS];
extern void blink_led(int times, int delay_time, CRGB color);
extern void led_running(int times, int delay_time, CRGB color);
void interruptSetup();
extern int make_safe(int val);
extern int EEPROMReadInt(int address);
extern void EEPROMWriteInt(int address, int value);
// extern void (*__shortPressFunc)(void);
// static void (*__mediumPressFunc)(void);
// extern void (*__longPressFunc)(void);
// class Button
// {
//     bool button_flag = false;
//     bool button_pressed = false;
//     uint32_t press_start = 0;
//     bool short_pressed = false;
//     // bool medium_pressed = false;
//     bool long_pressed = false;

// public:
//     Button();
//     void attachShortPress(void (*f)());
//     // void attachMediumPress(void (*f)());
//     void attachLongPress(void (*f)());
//     void start();
//     void stop();
//     void update();
// };

// extern Button myPB;

#endif