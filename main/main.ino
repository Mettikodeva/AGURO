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

void blink_led(int times, int delay_time,CRGB color){
{
    for (int i = 0; i < times*2; i++)
    {
        if (i % 2 == 0)
            for (int i = 0; i < NUM_LEDS; i++){
                leds[i] = CRGB::Black;
                FastLED.show();}
            
        else
            for (int i = 0; i < NUM_LEDS; i++){
                leds[i] = CRGB::Blue;
                FastLED.show();}
        delay(100);    
    }

}
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
        if (shift >= NUM_LEDS+2) shift = 0;
    }
    noInterrupts();
    digitalWrite(LED_BUILTIN, HIGH);
    
    // int min_[8] = {1023, 1023, 1023, 1023, 1023, 1023, 1023, 1023};
    // int max_[8] = {0, 0, 0, 0, 0, 0, 0, 0};
    // int min_2[8] = {1023, 1023, 1023, 1023, 1023, 1023, 1023, 1023};
    // int max_2[8] = {0, 0, 0, 0, 0, 0, 0, 0};
    //read line first, then read the background
    int line_[8] = {0, 0, 0, 0, 0, 0, 0, 0};
    int background_[8] = {0, 0, 0, 0, 0, 0, 0, 0};
    int counter = 0;
    while(!digitalRead(pb)){
        this->readsensorline();
        for (int i = 0; i < 8; i++)
        {
            line_[i] = sensors[i];
        }
        delay(10);
    }
    blink_led(3,150,CRGB::Blue);

    while (!digitalRead(pb))
    {
        this->readsensorline();
        for (int i = 0; i < 8; i++)
        {
            background_[i] = sensors[i];
        }
        delay(10);
    }
    blink_led(3,150,CRGB::Blue);
    // for (int i = 0; i < 400; i++)   
    // {
    //     this->readsensorline();
    //     int tmp[8] = {0, 0, 0, 0, 0, 0, 0, 0};
    //     for (int j = 0; j < 8; j++)
    //     {
    //         tmp[j] = sensors[j];

    //         if (tmp[j] < min_[j])
    //             min_[j] = tmp[j];
    //         if (tmp[j] > max_[j])
    //             max_[j] = tmp[j];
    //     }
    //     if (DEBUG)
    //     {
    //         Serial.print("*C");
    //         for (int i = 0; i < 8; i++){
    //             Serial.print(String(tmp[i])+",");}
    //         Serial.println("#");
    //     }
    //     delay(10);
    // }
    // this->calibrated = true;
    // for (int i = 0; i < 8; i++)
    // {
    //     min_line[i] = min_[i];
    //     max_line[i] = max_[i];
    // }
    digitalWrite(LED_BUILTIN, LOW);
    for(int i=0;i<8;i++){
        leds[i]=CRGB(0,0,0);
        FastLED.show();
        delay(100);
    }
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
    // IF THE SENSOR DETECT LINE THE VALUE WILL BE 1
    // ELSE 0
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
    float distance = duration / (30 * 2);
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
void Aguro::stop()
{
    this->start = false;
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

void centering(){
    
    
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
void Aguro::followUntil(char type, int speed)
{
    bool found_garis = false;
    int pass_line = 10;
    if (type == TJ)
    {
        for(int i = 0 ; i<100;i++){
            updateSensor();
            while(sensors[0]||sensors[7]){
                traceLine(speed);
                updateSensor();
                if(sensors[0]&&sensors[7])
                    found_garis = true;
                else
                    found_garis = false;
            }
            if(found_garis)
                break;
        }
        for(int i = 0; i< pass_line;i++){
            updateSensor();
            while (!sensors[0] || !sensors[7])
            {
                traceLine(speed);
                updateSensor();
                if (!sensors[0] && !sensors[7])
                    found_garis = true;
                else
                    found_garis = false;
            }
            if (found_garis)
                break;
        }
        for(int i = 0; i < 10; i++){
                updateSensor();
            while(sensors[0]||sensors[7]){
                updateSensor();
                motor(speed,speed);
            }
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
        

}

void Aguro::traceLine(int speed)
{

    float dl,dr, out;
    signed char err = 0 ;
    static float I;
    static signed char P,D,dErr, last_err;
    float Kp = 2, Ki =0.1, Kd = 2;
    int base_speedl = speed + 5;
    int base_speedr = speed;
    int max_speedl = 250;
    int max_speedr = 250;

    updateSensor();
    //    0 1  2  3 4  5  6 7
    if (sensors[3] ==0 && sensors[4] ==0)
    {
        err = 0;
        base_speedl += 25;
        base_speedr += 25;
    }
    else if (sensors[3] ==0 || sensors[4] ==0){
        if (sensors[3] ==0) err = -10; else err = 10;
        base_speedl+=15;
        base_speedr+=15;
        }
    
    else if (sensors[0] == 0 || sensors[7] == 0){
        if(sensors[0]) err = -30; else err = 30;
        base_speedl-=10;
        base_speedr-=10;}

    else if(sensors[1]==0|| sensors[6]==0)
        if(sensors[1]) err = -20; else err =20;

    else if (sensors[2] ==0 || sensors[5] ==0){
        if(sensors[2]) err = -13; else err =13;
        base_speedl+=10;
        base_speedr+=10;}
    else
        err = 0;
    dErr = err - last_err;
    last_err = err;
    P = err;
    I += err;
    out = (float)P * Kp + (float)I * Ki + (float)D * Kd;
    dl = (float)base_speedl - out;
    dr = (float)base_speedr + out;
    if (dl > max_speedl) dl = max_speedl;
    else if(dl<-max_speedl) dl = -max_speedl;
    if (dr > max_speedr) dr = max_speedr;
    else if(dr<-max_speedr) dr = -max_speedr;
    Serial.print("*P" + String(err)+","+String(dl)+","+String(dr)+"#");
    motor((char) dl,(char) dr);
}

void Aguro::motor(char dl, char dr)
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

// void Aguro::buttonInterrupt()
// {

// }

Aguro aguro;
Sensor mysensor;
void (*resetFunc)(void) = 0;

void setup()
{
    Serial.begin(115200);
    FastLED.addLeds<LED_TYPE, LED_PIN, COLOR_ORDER>(leds, NUM_LEDS);
    FastLED.setBrightness(BRIGHTNESS);
    aguro.init(true, &mysensor);
}
bool run = false;
int pb_counter=0;

void loop()
{
    if (digitalRead(pb) == HIGH)
    {
        run = !run;
        pb_counter++;
        if (pb_counter > 1000)
        { // set to counter equal to 3 s for resetting arduino
            resetFunc();
        }
    }
    else
    {
        
        if(pb_counter>100){ //stop the program
            aguro.stop();
        }
    }

    while (aguro.isStarted())
    {
        if (digitalRead(pb) == HIGH)
        {
            run = !run;
            pb_counter++;
            if (pb_counter > 1000)
            { // set to counter equal to 3 s for resetting arduino
                resetFunc();
            }
        }
        else{
        
        if(pb_counter>100){ //stop the program
            // aguro.stop();
        }
    }
    if(run){
        aguro.traceLine();
        Serial.println("tracing line");
    }

    }
    }
