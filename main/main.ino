#include <FastLED.h>
#include "sensor.h"
#include "aguro.h"
#define LED_PIN 12
#define LED_TYPE WS2812B
#define NUM_LEDS 8
#define COLOR_ORDER RGB
#define BRIGHTNESS 30
CRGB leds[NUM_LEDS];
#define UPDATES_PER_SECOND 100
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

char Sensor::readsensorline(int sample_count)
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
        Serial.println("Calibrating line sensor...");
    }
    noInterrupts();
    digitalWrite(LED_BUILTIN, HIGH);
    int min_[8] = {1023, 1023, 1023, 1023, 1023, 1023, 1023, 1023};
    int max_[8] = {0, 0, 0, 0, 0, 0, 0, 0};
    int counter = 0;
    for (int i = 0; i < 500; i++)
    {
        this->readsensorline();
        int tmp[8] = {0, 0, 0, 0, 0, 0, 0, 0};
        for (int j = 0; j < 8; j++)
        {
            tmp[j] = sensors[j];

            if (tmp[j] < min_[j])
                min_[j] = tmp[j];
            if (tmp[j] > max_[j])
                max_[j] = tmp[j];
        }
        if (DEBUG)
        {
            Serial.print("*C");
            for (int i = 0; i < 8; i++){
                Serial.print(String(tmp[i])+",");}
            Serial.println("#");
        }
    }
    this->calibrated = true;
    for (int i = 0; i < 8; i++)
    {
        min_line[i] = min_[i];
        max_line[i] = max_[i];
    }
    digitalWrite(LED_BUILTIN, LOW);
    interrupts();
    Serial.println("Calibrated");
    if (DEBUG)
    {
        Serial.print("min: ");
        for (int i = 0; i < 8; i++)
            Serial.print("\t" + String(min_[i]));
        Serial.println();
        Serial.print("max: ");
        for (int i = 0; i < 8; i++)
            Serial.print("\t" + String(max_[i]));
        Serial.println();
        for (int i = 0; i < 8; i++)
        {
            Serial.println("active : val > " + String((max_line[i] - min_line[i]) / 2 + min_line[i]));
        }
    }
}

bool Sensor::readlinebool(int index)
{
    readsensorline();
    if (this->calibrated)
    {
        if (sensors[index] > (max_line[index] - min_line[index]) / 2 + min_line[index])
            return 1;
        else
            return 0;
    }
    else
    {
        if (sensors[index] > 800)
            return 1;
        else
            return 0;
        Serial.println("Not calibrated");
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
    if (DEBUG)
    {
        Serial.print("Distance: ");
        Serial.println(distance);
    }
    return distance;
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

void Aguro::buttonInterrupt()
{
    this->start = !this->start;
    pinMode(LED_BUILTIN, HIGH);
    delay(100);
    pinMode(LED_BUILTIN, LOW);
    delay(100);
    pinMode(LED_BUILTIN, HIGH);
    delay(100);
    pinMode(LED_BUILTIN, LOW);
    delay(100);
    pinMode(LED_BUILTIN, HIGH);
    delay(100);
    pinMode(LED_BUILTIN, LOW);
}

bool Aguro::isStarted()
{
    return this->start;
}

void Aguro::init(bool debug, Sensor *sensor)
{
    pinMode(IN1, OUTPUT);
    pinMode(IN2, OUTPUT);
    pinMode(IN3, OUTPUT);
    pinMode(IN4, OUTPUT);
    pinMode(ENA, OUTPUT);
    pinMode(ENB, OUTPUT);
    pinMode(Relay, OUTPUT);
    //    attachInterrupt(PB,buttonInterrupt, LOW);
    s = sensor;
    s->init(debug);
    this->DEBUG = debug;
    s->calibrateLine();
}
void Aguro::updateSensor()
{
    noInterrupts();
    Serial.print("*S");
    for (int i = 0; i < 8; i++)
    {
        sensors[i] = s->readlinebool(i);
        if (sensors[i] == 0)
        {
            leds[i] = CRGB(255, 0, 0);
            FastLED.show();
        }
        else
        {
            leds[i] = CRGB(0, 255, 0);
            FastLED.show();
        }
        Serial.print(String(sensors[i]) + ",");
    }
    Serial.println("#");
    interrupts();
}

// moving the robot
void Aguro::followUntil(char type)
{
    while (true)
    {
        updateSensor();
        if (type == TJ)
        {
            if (sensors[0] && sensors[1] && sensors[6] && sensors[7])
            {
                break;
            }
        }
        else if (type == FR)
        {
            if (sensors[6] && sensors[7])
            {
                break;
            }
        }
        else if (type == FL)
        {
            if (sensors[0] && sensors[1])
            {
                break;
            }
        }
        traceLine();
    }
}

void Aguro::traceLine()
{

    int dl = 50, dr = 50;
    int err;
    int P,D,dErr,Kp = 1, Ki = 0, Kd = 0;
    int base_speedl = 150;
    int base_speedr = 150;
    int max_speedl = 240;
    int max_speedr = 240;

    updateSensor();
    //    0 1 2 3 4 5 6 7
    if (sensors[3] == 1 && sensors[4] == 1)
    {
        err = 0;

    }

    else if (sensors[3] == 1 || sensors[4] == 1)
        if (sensors[3] == 1) err = -5; else err = 5;
    
    else if (sensors[2] == 1 || sensors[5] == 1)
        if(sensors[2]) err = -10; else err = 10;

    else if(sensors[1]==1|| sensors[6]==1)
        if(sensors[1]) err = -15; else err = 15;
    
    else if (sensors[0] == 1 || sensors[7] == 1)
        if(sensors[0]) err = -20; else err = 20;
    else
    {

    }
    dErr = err - last_err;
    last_err = err;
    P = err;
    I += err;
    int speed = P*Kp + I*Ki + D*Kd;
    dl = base_speedl + speed;
    dr = base_speedr - speed;
    if (dl > max_speedl) dl = max_speedl;
    if (dr > max_speedr) dr = max_speedr;
    Serial.print("*P" + String(err)+","+String(dl)+","+String(dr)+"#");
    //     Serial.print("err: " + String(err));
    // Serial.println("dl: "+String(dl)+"\t dr: "+String(dr));
    motor(dl,dr);
}

void Aguro::motor(int dl, int dr)
{
    float dist = s->read_ultrasonic();
    if (DEBUG)
        Serial.println("*D" + String(dist)+"#");
    if (dist < 20)
    {
        dl = 0;
        dr = 0;
        Serial.println("stopped, please move the obstacle!");
    }
    if (dl < 0)
    {
        digitalWrite(IN1, HIGH);
        digitalWrite(IN2, LOW);

        analogWrite(ENA, -dl);
    }
    else
    {
        digitalWrite(IN1, LOW);
        digitalWrite(IN2, HIGH);
        analogWrite(ENA, dl);
    }
    if (dr < 0)
    {
        digitalWrite(IN3, HIGH);
        digitalWrite(IN4, LOW);
        analogWrite(ENB, -dr);
    }
    else
    {
        digitalWrite(IN3, LOW);
        digitalWrite(IN4, HIGH);
        analogWrite(ENB, dr);
    }
}

Aguro aguro;
Sensor mysensor;
void setup()
{
    Serial.begin(115200);
    aguro.init(true, &mysensor);
    FastLED.addLeds<LED_TYPE, LED_PIN, COLOR_ORDER>(leds, NUM_LEDS);
    FastLED.setBrightness(BRIGHTNESS);
}

void loop()
{
    if (aguro.isStarted())
    {
        aguro.traceLine();
        Serial.println("tracing line");
        delay(100);
    }
}
