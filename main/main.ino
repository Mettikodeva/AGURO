#include <FastLED.h>
#include "sensor.h"
#include "aguro.h"
#define LED_PIN 12
#define LED_TYPE WS2812B
#define NUM_LEDS 8
#define COLOR_ORDER RGB
#define BRIGHTNESS 65
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
    for (int i = 0; i < 1000; i++)
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
                  for(int i=0;i<8;i++)
                    Serial.print("\t"+String(tmp[i]));
                   Serial.println();
                    
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
        if(DEBUG){
            Serial.print("min: ");
            for(int i=0; i<8;i++)
              Serial.print("\t"+String(min_[i]));
             Serial.println();
            Serial.print("max: ");
            for(int i=0; i<8;i++)
              Serial.print("\t"+String(max_[i]));
            Serial.println();
        }
    for (int i = 0; i < 8; i++)
    {
        Serial.println("active : val > " + String((max_line[i] - min_line[i]) / 2 + min_line[i]));
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
    float distance = duration / (29.1*2);
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
    for (int i = 0; i < 8; i++)
    {
        sensors[i] = s->readlinebool(i);
        if(sensors[i] == 0){
          leds[i] = CRGB(255, 0, 0);
          FastLED.show();
        }
        else{
          leds[i] = CRGB(0, 255, 0);
          FastLED.show();
        }
        Serial.print("\t"+String(sensors[i]));

        
    }
    Serial.println();
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
    updateSensor();
    //    0 1 2 3 4 5 6 7
    if (sensors[3] == 1 && sensors[4] == 1){
        Serial.println("robot maju");
          motor(255, 255);
    }
      
    else if (sensors[3] == 1 || sensors[4] == 1)
    {
        if (sensors[4] == 1)
            motor(230, 200);
        else
            motor(200, 230);
    }
    else if (sensors[2] == 1 || sensors[1] == 1)
        motor(170, 230);
    else if (sensors[6] == 1 || sensors[5] == 1)
        motor(230, 170);
    else if (sensors[0] == 1)
        motor(120, 230);
    else if (sensors[7] == 1)
        motor(230, 120);
    else{
      motor(120,120);
    }
    
}

void Aguro::motor(int dl, int dr)
{
    float dist = s->read_ultrasonic();
    if (DEBUG)
        Serial.println("dist : " + String(dist));
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
void setup(){
    Serial.begin(115200);
    aguro.init(true, &mysensor);
    FastLED.addLeds<LED_TYPE, LED_PIN, COLOR_ORDER>(leds, NUM_LEDS);
    FastLED.setBrightness(BRIGHTNESS);
}

void loop(){
    if(aguro.isStarted()){
        aguro.traceLine();
        Serial.println("tracing line");
    }
}
