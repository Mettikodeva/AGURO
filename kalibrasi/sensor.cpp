#include "sensor.h"

void Sensor::init(bool debug)
{
    pinMode(PushButton, INPUT_PULLUP);
    pinMode(Trig, OUTPUT);
    pinMode(Echo, INPUT);
    pinMode(MUXData, INPUT);
    pinMode(MUXPinA, OUTPUT);
    pinMode(MUXPinB, OUTPUT);
    pinMode(MUXPinC, OUTPUT);
    this->DEBUG = debug;
}

void Sensor::setMux(int index)
{
    digitalWrite(MUXPinA, MUX[index][0]);
    digitalWrite(MUXPinB, MUX[index][1]);
    digitalWrite(MUXPinC, MUX[index][2]);
}

void Sensor::calibrateLine()
{
    int shift = 0;
    for (int j = 0; j < 30; j++)
    {
        for (int i = 0; i < NUM_LEDS; i++)
        {
            if (i <= shift && i > shift - 4)
            {
                leds[i] = CRGB(255, 255, 0);
                FastLED.show();
            }
            else
            {
                leds[i] = CRGB(0, 0, 0);
                FastLED.show();
            }
        }
        shift++;
        if (shift >= NUM_LEDS + 2)
            shift = 0;
    }
    digitalWrite(LED_BUILTIN, HIGH);
    // read line first, then read the background
    int line_[12] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
    int background_[12] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
    // int counter = 0;
    // blink_led(3, 200, CRGB::Red);
    Serial.println("calibrating");
    Serial.println("line");
    while (digitalRead(PushButton))
    {
        for (int i = 0; i < 8; i++)
        {
            setMux(i);
            delayMicroseconds(10);
            line_[i] = analogRead(MUXData);
            Serial.print(line_[i]);
            Serial.print(" ");
        }
        line_[8] = analogRead(SpinFL);
        Serial.print(line_[8]);
            Serial.print(" ");
        line_[9] = analogRead(SpinFR);
        Serial.print(line_[9]);
            Serial.print(" ");
        Serial.println();
        delay(10);
    }

    Serial.println("background");

    blink_led(3,150,CRGB::Blue);

    Serial.println("background");
    while (digitalRead(PushButton))
    {
        for (int i = 0; i < 8; i++)
        {
            setMux(i);
            delayMicroseconds(10);
            background_[i] = analogRead(MUXData);
            Serial.print(background_[i]);
            Serial.print(" ");
        }
        background_[8] = analogRead(SpinFL);
        Serial.print(background_[8]);
            Serial.print(" ");;
        background_[9] = analogRead(SpinFR);
        Serial.print(background_[9]);
            Serial.print(" ");
        Serial.println();
        delay(10);
    }
    this->calibrated = true;
    if (line_[0] < background_[0])
        line_high = false;
    else
        line_high = true;
    for (int i = 0; i < 10; i++)
    {
        if (line_high)
        {
            max_line[i] = line_[i];
            min_line[i] = background_[i];
        }
        else
        {
            max_line[i] = background_[i];
            min_line[i] = line_[i];
        }
    }
    // save_calibration();
    blink_led(3,150,CRGB::Red);
    while (digitalRead(PushButton))
    {
        line_[10] = analogRead(SpinBL);
        Serial.print(line_[10]);
            Serial.print(" ");
        line_[11] = analogRead(SpinBR);
        Serial.print(line_[11]);
            Serial.print(" ");
        Serial.println();
        delay(10);
    }
    blink_led(3,150,CRGB::Red);
    while (digitalRead(PushButton))
    {
        background_[10] = analogRead(SpinBL);
        Serial.print(background_[10]);
            Serial.print(" ");
        background_[11] = analogRead(SpinBR);
        Serial.print(background_[11]);
            Serial.print(" ");
        Serial.println();
        delay(10);
    }
    blink_led(5,150,CRGB::Red);
    this->calibrated = true;

    for (int i = 10; i < 12; i++)
    {
        if (line_high)
        {
            max_line[i] = line_[i];
            min_line[i] = background_[i];
        }
        else
        {
            max_line[i] = background_[i];
            min_line[i] = line_[i];
        }
    }
    save_calibration();
    shift = 0;
    for (int j = 0; j < 30; j++)
    {
        for (int i = 0; i < NUM_LEDS; i++)
        {
            if (i <= shift && i > shift - 4)
            {
                leds[i] = CRGB(25, 255, 0);
                FastLED.show();
            }
            else
            {
                leds[i] = CRGB(0, 0, 0);
                FastLED.show();
            }
        }
        shift++;
        if (shift >= NUM_LEDS + 2)
            shift = 0;
    }

    digitalWrite(LED_BUILTIN, LOW);
    for (int i = 0; i < 8; i++)
    {
        leds[i] = CRGB(0, 0, 0);
        FastLED.show();
        delay(100);
    }
}

void Sensor::save_calibration()
{
    for (int i = 0; i < 12; i++)
    {
        EEPROMWriteInt(i * sizeof(int), max_line[i]);
        EEPROMWriteInt(i * sizeof(int) + 12 * sizeof(int), min_line[i]);
    }
}

void Sensor::read_calibration()
{
    for (int i = 0; i < 12; i++)
    {
        max_line[i] = EEPROMReadInt(i * sizeof(int));
        min_line[i] = EEPROMReadInt(i * sizeof(int) + 12 * sizeof(int));
    }
}

bool Sensor::is_line_high()
{
    return this->line_high;
}
// bool Sensor::readlinebool(int index)
// {
//     // IF THE SENSOR DETECT LINE THE VALUE WILL BE 1
//     // ELSE 0
//     noInterrupts();
//     setMux(index);
//     delayMicroseconds(10);
//     sensors[index] = analogRead(MUXData);
//     if (this->calibrated)
//     {
//         if (sensors[index] > (max_line[index] - min_line[index]) / 2 + min_line[index])
//             if (line_high)
//                 return 1;
//             else
//                 return 0;
//         else if (line_high)
//             return 0;
//         else
//             return 1;
//     }
//     else
//     {
//         if (sensors[index] > 750)
//             if (line_high)
//                 return 1;
//             else
//                 return 0;
//         else if (line_high)
//             return 0;
//         else
//             return 1;
//     }
// }

// read ultrasonic and return in cm
// float Sensor::read_ultrasonic()
// {
//     digitalWrite(Trig, LOW);
//     delayMicroseconds(2);
//     digitalWrite(Trig, HIGH);
//     delayMicroseconds(10);
//     digitalWrite(Trig, LOW);
//     float duration = pulseIn(Echo, HIGH);
//     float distance = duration / (29.1 * 2);
//     return distance;
// }