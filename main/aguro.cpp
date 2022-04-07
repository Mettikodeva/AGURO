#include "aguro.h"

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
    attachInterrupt(PB, this->buttonInterrupt, LOW);
    s = sensor;
    s->init(debug);
    this->DEBUG = debug;
}
void Aguro::updateSensor(){
    nointerrupts();
    for (int i = 0; i < 8; i++)
    {
        sensors[i] = s->readlinebool(i);
    }
    interrupts();
}

// moving the robot
void Aguro::followUntil(char type){
    while(true){
        updateSensor();
        if(type == TJ){
            if(sensors[0] && sensors[1] && sensors[6] && sensors[7]){
                break;
            }
        }
        else if(type == FR){
            if(sensors[6] && sensors[7] ){
                break;
            }
        }
        else if(type == FL){
            if(sensors[0] && sensors[1] ){
                break;
            }
        }
        traceLine();
    }
}
void Aguro::traceLine(){
    updateSensor();
    //    0 1 2 3 4 5 6 7
    if (sensors[3] == 0 && sensors[4] == 0)
        motor(255, 255);
    else if (sensors[3] == 0 || sensors[4] == 0)
    {
        if (sensors[4] == 0)
            motor(255, 200);
        else
            motor(200, 255);
    }
    if (sensors[2] == 0 || sensors[1] == 0)
        motor(150, 255);
    if (sensors[6] == 0 || sensors[5] == 0)
        motor(255, 150);
    if (sensors[0] == 0)
        motor(100, 255);
    if (sensors[7] == 0)
        motor(255, 100);
}

void Aguro::motor(int dl, int dr, float time=0)
{
    float dist = s->read_ultrasonic();
    if(DEBUG)Serial.println("dist : "+String(dist));
    if (dist < 20)
    {
        dl = 0;
        dr = 0;
        Serial.println("stopped, please move the obstacle!");
    }
    if (dl < 0)
    {
        digitalWrite(IN1, LOW);
        digitalWrite(IN2, HIGH);
        analogWrite(ENA, -dl);
    }
    else
    {
        digitalWrite(IN1, HIGH);
        digitalWrite(IN2, LOW);
    }
    if (dr < 0)
    {
        digitalWrite(IN3, LOW);
        digitalWrite(IN4, HIGH);
        analogWrite(ENB, -dr);
    }
    else
    {
        digitalWrite(IN3, HIGH);
        digitalWrite(IN4, LOW);
    }
}

