#include "sensor.h"
#include "Arduino.h"
void Sensor::init(bool debug){
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
    nointerrupts();
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
    if(this->DEBUG){
        Serial.println("Calibrating line sensor...");
    }
    nointerrupts();
    digitalWrite(LED_BUILTIN, HIGH);
    int min_[8] = {1023, 1023, 1023, 1023, 1023, 1023, 1023, 1023};
    int max_[8] = {0, 0, 0, 0, 0, 0, 0, 0};
    int counter = 0;
    for (int i = 0; i < 500; i++){
        this->readsensorline();
        int tmp[8] = {0,0,0,0,0,0,0,0};
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
            Serial.println("%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d",tmp[0],tmp[1],tmp[2],tmp[3],tmp[4],tmp[5],tmp[6],tmp[7]);
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
        Serial.println("%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d", min_[0], min_[1], min_[2], min_[3], min_[4], min_[5], min_[6], min_[7]);
        Serial.print("max: ");
        Serial.println("%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d", min_[0], min_[1], min_[2], min_[3], min_[4], min_[5], min_[6], min_[7]);
        for(int i =0; i<8; i++){
            Serial.println("active : val > " + String((max_line[i] - min_line[i]) / 2 + min_line[i]));      
        }
    }

}

bool Sensor::readlinebool(int index){
    readsensorline();
    if (this->calibrated)
    {
        if (sensors[index] > (max_line[index] - min_line[index]) / 2 + min_line[index]) return 1;
        else return 0;
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
    float distance = (duration / 2) / 34.3;
    if (DEBUG)
    {
        Serial.print("Distance: ");
        Serial.println(distance);
    }
    return distance;
}
