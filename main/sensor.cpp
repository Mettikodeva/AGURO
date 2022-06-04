#include "sensor.h"

void Sensor::init(bool debug)
{
    pinMode(Trig, OUTPUT);
    pinMode(Echo, INPUT);
    pinMode(sp1, INPUT);
    pinMode(sp2, INPUT);
    pinMode(sp3, INPUT);
    pinMode(sp4, INPUT);
    pinMode(sp5, INPUT);
    pinMode(sp6, INPUT);
    pinMode(sp7, INPUT);
    pinMode(sp8, INPUT);
    this->DEBUG = debug;
}

char Sensor::readsensorline()
{
    noInterrupts();
    sensors[0] = analogRead(sp1);
    sensors[1] = analogRead(sp2);
    sensors[2] = analogRead(sp3);
    sensors[3] = analogRead(sp4);
    sensors[4] = analogRead(sp5);
    sensors[5] = analogRead(sp6);
    sensors[6] = analogRead(sp7);
    sensors[7] = analogRead(sp8);
    interrupts();
}

void Sensor::calibrateLine()
{
    if (this->DEBUG)
    {
        // Serial.println("Calibrating line sensor...");
    }
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
            delay(3);
            // FastLED.show();
        }
        shift++;
        if (shift >= NUM_LEDS + 2)
            shift = 0;
    }
    noInterrupts();
    digitalWrite(LED_BUILTIN, HIGH);
    // read line first, then read the background
    int line_[8] = {0, 0, 0, 0, 0, 0, 0, 0};
    int background_[8] = {0, 0, 0, 0, 0, 0, 0, 0};
    int counter = 0;
    while (!digitalRead(PB))
    {
        this->readsensorline();
        for (int i = 0; i < 8; i++)
        {
            line_[i] = sensors[i];
            Serial.println("baca line " + String(sensors[i]));
        }
        delay(10);
    }
    blink_led(3, 150, CRGB::Blue);
    while (!digitalRead(PB))
    {
        this->readsensorline();
        for (int i = 0; i < 8; i++)
        {
            background_[i] = sensors[i];
            // Serial.println("baca background " + String(sensors[i]));
        }
        delay(10);
    }
    blink_led(3, 150, CRGB::Blue);
    this->calibrated = true;
    if (line_[0] < background_[0])
        line_high = false;
    else
        line_high = true;
    for (int i = 0; i < 8; i++)
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

    digitalWrite(LED_BUILTIN, LOW);
    for (int i = 0; i < 8; i++)
    {
        leds[i] = CRGB(0, 0, 0);
        FastLED.show();
        delay(100);
    }
    interrupts();
    if (DEBUG)
    {
        for (int i = 0; i < 8; i++)
            // Serial.print("\t" + String(line_[i]));
            // Serial.println();
            // Serial.print("bg: ");
            for (int i = 0; i < 8; i++)
                // Serial.print("\t" + String(background_[i]));
                // Serial.println();
                for (int i = 0; i < 8; i++)
                {
                    // Serial.println("active : val > " + String((max_line[i] - min_line[i]) / 2 + min_line[i]));
                }
    }
}

bool Sensor::is_line_high()
{
    return this->line_high;
}

bool Sensor::readlinebool(int index)
{
    // IF THE SENSOR DETECT LINE THE VALUE WILL BE 1
    // ELSE 0
    readsensorline();
    if (this->calibrated)
    {
        if (sensors[index] > (max_line[index] - min_line[index]) / 2 + min_line[index])
            if (line_high)
                return 1;
            else
                return 0;
        else if (line_high)
            return 0;
        else
            return 1;
    }
    else
    {
        if (sensors[index] > 800)
            return 1;
        else
            return 0;
        // Serial.println("Not calibrated");
    }
}

// read ultrasonic and return in cm
float Sensor::read_ultrasonic()
{
    digitalWrite(Trig, LOW);
    delayMicroseconds(2);
    digitalWrite(Trig, HIGH);
    delayMicroseconds(10);
    digitalWrite(Trig, LOW);
    float duration = pulseIn(Echo, HIGH);
    float distance = duration / (34.1 * 2);
    if (DEBUG)
    {
        // Serial.print("Distance: ");
        // Serial.println(distance);
    }
    return distance;
}