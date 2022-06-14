#include "definitions.h"

void (*resetFunc)(void) = 0;
CRGB leds[NUM_LEDS] = {
    CRGB(0, 0, 0),
    CRGB(0, 0, 0),
    CRGB(0, 0, 0),
    CRGB(0, 0, 0),
    CRGB(0, 0, 0),
    CRGB(0, 0, 0),
    CRGB(0, 0, 0),
    CRGB(0, 0, 0),
};

int EEPROMReadInt(int address)
{
    long byte1 = EEPROM.read(address);
    long byte2 = EEPROM.read(address + 1);
    return ((byte2 << 0) & 0xFF) + ((byte1 << 8) & 0xFFFF);
}

void EEPROMWriteInt(int address, int value)
{
    byte byte1 = value >> 8;
    byte byte2 = value & 0xFF;
    EEPROM.write(address, byte1);
    EEPROM.write(address + 1, byte2);
}

void interruptSetup()
{
    // Initializes Timer2 to throw an interrupt every 2mS.
    // TCCR0A = 0b00100010; // DISABLE PWM ON DIGITAL PINS 3 AND 11, AND GO INTO CTC MODE
    // TCCR0B = 0b00000101; // DON'T FORCE COMPARE, 1024 PRESCALER
    // OCR0A = 194;         // SET THE TOP OF THE COUNT TO 194 FOR 40Hz SAMPLE RATE
    // TIMSK0 = 0b00000011; // ENABLE INTERRUPT ON MATCH BETWEEN TIMER2 AND OCR2A
    // TIFR0 = 0b00000011;  // CLEAR THE INTERRUPT FLAG
    // sei();               // MAKE SURE GLOBAL INTERRUPTS ARE ENABLED
}

void blink_led(int times, int delay_time, CRGB color)
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

void led_running(int times, int delay_time, CRGB color)
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
int make_safe(int val)
{
    if (val > 255)
        return 255;
    else if (val < 0)
        return 0;
    else
        return val;
}
// Button::Button()
// {
//     interruptSetup();
//     cli();
// }
// void Button::start()
// {
//     sei();
// }
// void Button::stop()
// {
//     cli();
// }
// void Button::update()
// {
//     uint32_t time = 0;
//     if (digitalRead(PushButton))
//     {
//         if (button_pressed)
//         {
//             uint32_t temp = millis();
//             if (temp - press_start > RESET_PRESS && button_flag == false)
//             {
//                 button_flag = true;
//                 blink_led(5, 300, CRGB(230, 00, 0));
//                 resetFunc();
//             }
//             else
//             {
//                 time = temp - press_start;
//             }
//         }
//         else
//         {
//             button_pressed = true;
//             press_start = millis();
//         }
//     }
//     else
//     {
//         button_pressed = false;
//         button_flag = false;
//         short_pressed = false;
//         // medium_pressed = false;
//         long_pressed = false;
//     }

//     if (time > LONG_PRESS)
//     {
//         blink_led(5, 300, CRGB(200, 200, 0));
//         __longPressFunc();
//         long_pressed = true;
//     }
//     else if (time > MEDIUM_PRESS)
//     {
//         // blink_led(5, 300, CRGB(150, 250, 0));
//         // __mediumPressFunc();
//         // medium_pressed = true;
//     }
//     else if (time > SHORT_PRESS)
//     {
//         blink_led(5, 300, CRGB(0, 250, 0));
//         __shortPressFunc();
//         short_pressed = true;
//     }
// }
// void Button::attachShortPress(void (*f)(void))
// {
//     __shortPressFunc = f;
// }
// // void Button::attachMediumPress(void (*f)())
// // {
// //     __mediumPressFunc = f;
// // }
// void Button::attachLongPress(void (*f)(void))
// {
//     __longPressFunc = f;
// }

// // Button myPB = Button();
// // ISR(TIMER0_COMPA_vect)
// // {
// //     myPB.update();
// // }