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

    read_calibration();
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
    int counter = 0;
    uint16_t start = millis();
    bool wanna_calibrate = false;
    while (millis() - start < 3000)
    {
        blink_led(3, 50, CRGB::Red);

        if (!digitalRead(PushButton))
        {
            wanna_calibrate = true;
            break;
        }
        else
        {
            wanna_calibrate = false;
        }
    }
    Serial.println("keluar");
    if (!wanna_calibrate)
    {
        this->calibrated = true;
        read_calibration();
        blink_led(3,150,CRGB::Green);   
        return;
    }
    blink_led(3, 200, CRGB::Red);
    while (digitalRead(PushButton))
    {
        for (int i = 0; i < 8; i++)
        {
            setMux(i);
            delayMicroseconds(10);
            line_[i] = analogRead(MUXData);
        }
        line_[8] = analogRead(SpinFL);
        line_[9] = analogRead(SpinFR);
        delay(10);
    }

    blink_led(3, 150, CRGB::Blue);
    while (digitalRead(PushButton))
    {
        for (int i = 0; i < 8; i++)
        {
            setMux(i);
            delayMicroseconds(10);
            background_[i] = analogRead(MUXData);
        }
        background_[8] = analogRead(SpinFL);
        background_[9] = analogRead(SpinFR);
        delay(10);
    }
    blink_led(3, 150, CRGB::Blue);
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
    shift = 0;
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
    blink_led(3, 200, CRGB::Red);
    while (digitalRead(PushButton))
    {
        line_[10] = analogRead(SpinBL);
        line_[11] = analogRead(SpinBR);
        delay(10);
    }
    blink_led(3, 150, CRGB::Blue);
    while (digitalRead(PushButton))
    {
        background_[10] = analogRead(SpinBL);
        background_[11] = analogRead(SpinBR);
        delay(10);
    }
    blink_led(3, 150, CRGB::Blue);
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
    blink_led(5,150,CRGB::Red);
    blink_led(3,100,CRGB::Black);
}

bool Sensor::is_line_high()
{
    return this->line_high;
}
bool Sensor::readlinebool(int index)
{
    // IF THE SENSOR DETECT LINE THE VALUE WILL BE 1
    // ELSE 0
    int compare = 0;
    if(index<8){
    setMux(index);
    delayMicroseconds(10);
    sensors[index] = analogRead(MUXData);
    if(index == 0){
        // Serial.println("sensor 1 : " +String(sensors[0]));
    }
        compare = sensors[index] > (max_line[index] - min_line[index]) / 2 + min_line[index];
        // Serial.println("s"+String(index)+", val : "+String(sensors[index]));
        if (sensors[index] > (max_line[index] - min_line[index]) / 2 + min_line[index])
            if (line_high)
                return 1;
            else
                return 0;
        else {
            if (line_high)
                return 0;
            else
                return 1;}
    }
    else{
        switch(index)
        {
            case SFL:
                sensors[index] = analogRead(SpinFL);
                if (sensors[index] > 600)
                    if (line_high)
                        return 1;
                    else
                        return 0;
                else {
                    if (line_high)
                        return 0;
                    else
                        return 1;}
            case SFR:
                sensors[index] = analogRead(SpinFR);
                if (sensors[index] > 600)
                    if (line_high)
                        return 1;
                    else
                        return 0;
                else {
                    if (line_high)
                        return 0;
                    else
                        return 1;}
            case SBL:
                sensors[index] = analogRead(SpinBL);
                if (sensors[index] > 600)
                    if (line_high)
                        return 1;
                    else
                        return 0;
                else {
                    if (line_high)
                        return 0;
                    else
                        return 1;}
            case SBR:
                sensors[index] = analogRead(SpinBR);
                if (sensors[index] > 400)
                    if (line_high)
                        return 1;
                    else
                        return 0;
                else {
                    if (line_high)
                        return 0;
                    else
                        return 1;}
        }

        // compare = (min_line[index] - max_line[index]) / 2 + max_line[index];
        // Serial.println("s"+String(index)+", val:"+ String(compare));
        // Serial.println("compare : "+ String(compare));
        
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
    float distance = duration / (29.1 * 2);
    return distance;
}