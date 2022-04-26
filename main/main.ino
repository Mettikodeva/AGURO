#include "LED.h"
#include "sensor.h"
#include "aguro.h"

void blink_led(int times, int delay_time,CRGB color)
{
    for (int i = 0; i < times*2; i++)
    {
        if (i % 2 == 0)
            for (int i = 0; i < NUM_LEDS; i++){
                leds[i] = CRGB::Black;
                FastLED.show();
                }
            
        else
            for (int i = 0; i < NUM_LEDS; i++){
                leds[i] = CRGB::Blue;
                FastLED.show();
            }
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
    while(!digitalRead(PB)){
        this->readsensorline();
        for (int i = 0; i < 8; i++)
        {
            line_[i] = sensors[i];
            Serial.println("baca line " + String(sensors[i]));
        }
        delay(10);
    }

    blink_led(3,150,CRGB::Blue);

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
            // Serial.print("*C");
    //         for (int i = 0; i < 8; i++){
                // Serial.print(String(tmp[i])+",");}
            // Serial.println("#");
    //     }
    //     delay(10);
    // }
    this->calibrated = true;
    if(line_[0] < background_[0])line_high=false;
    else line_high=true;
    for (int i = 0; i < 8; i++)
    {
        if(line_high){
            max_line[i] = line_[i];
            min_line[i] = background_[i];
        }
        else{
            max_line[i] = background_[i];
            min_line[i] = line_[i];
        }
    }

    digitalWrite(LED_BUILTIN, LOW);
    for(int i=0;i<8;i++){
        leds[i]=CRGB(0,0,0);
        FastLED.show();
        delay(100);
    }
    interrupts();
    // Serial.println("Calibrated");
    if (DEBUG)
    {
        // Serial.print("line: ");
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
bool Sensor::is_line_high(){
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
            if(line_high)return 1;
            else return 0;
        else
            if(line_high)return 0;
            else return 1;
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
int make_safe(int val)
{
    if (val > 255)
        return 255;
    else if (val < 0)
        return 0;
    else
        return val;
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

void Aguro::centering(){
    updateSensor();
    float dl, dr, out;
    signed char err = 0;
    static float I;
    static signed char P, D, dErr, last_err;
    float Kp = 3, Ki = 0.05, Kd = 2;
    int max_speedl = 250;
    int max_speedr = 250;
    int counter = 0;
    while(!sensors[3]&&!sensors[4] && counter > 3){
        updateSensor();
        counter++;
        if (sensors[3] == 1 && sensors[4] == 1)
        {
            break;
            err = 0;
            line_found = true;
        }
        else if (sensors[3] == 1 || sensors[4] == 1)
        {
            if (sensors[3] == 1)
                err = -7;
            else
                err = 7;
            line_found = true;
        }

        else if (sensors[0] == 1 || sensors[7] == 1)
        {
            if (sensors[0])
                if (line_found == true)
                    err = -60;
                else
                    err = 80;
            else if (line_found == true)
                err = 60;
            else
                err = -80;
            line_found = true;
        }

        else if (sensors[1] == 1 || sensors[6] == 1)
        {
            if (sensors[1])
                err = -40;
            else
                err = 40;
            counter++;
            line_found = true;
        }

        else if (sensors[2] == 1 || sensors[5] == 1)
        {
            if (sensors[2])
                err = -25;
            else
                err = 25;
            counter+=2;
            line_found = true;
        }

        dErr = err - last_err;
        if (line_found)
            last_err = err;
        P = Kp * err;
        I += err;
        D = Kd * dErr;
        out = (float)P + (float)I * Ki + (float)D;
        dl = (float)+out;
        dr = (float)-out;
        if (dl > max_speedl)
            dl = max_speedl;
        else if (dl < -max_speedl)
            dl = -max_speedl;
        if (dr > max_speedr)
            dr = max_speedr;
        else if (dr < -max_speedr)
            dr = -max_speedr;
        // Serial.print("*P" + String(err) + "," + String(dl) + "," + String(dr) + "#");
        motor(dl, dr);
    }
}

void Aguro::updateSensor()
{
    noInterrupts();
    // Serial.print("*S");
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
    // Serial.println("#");
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
            // break;
        }
    }
    else if (type == FL)    
    {
        if (sensors[0] && sensors[1])
        {
            // break;
        }
    }
}

void Aguro::traceLine(int speed)
{

    float dl,dr, out;
    signed char err = 0 ;
    static float I;
    static signed char P,D,dErr, last_err;
    float Kp = 2, Ki =0.05, Kd = 2;
    int base_speedl = speed;
    int base_speedr = speed;    
    int max_speedl = 240;
    int max_speedr = 240;

    updateSensor();
    //    0 1  2  3 4  5  6 7
    if(after_turn>0){
        delay(10);
        centering();
        after_turn=0;
    }

    if ((sensors[0] == 1 && sensors[1] == 1 && sensors[2] == 1&&sensors[3]==1) || (sensors[7] == 1 && sensors[6] == 1 && sensors[5] == 1&&sensors[4]==1)){
        if(sensors[0] == 1){ 
            err = -200;
            base_speedl -= 100;
        }
        else{
            base_speedr -= 100;
            err = 200; 
        }
        after_turn++;
    }
    else if (sensors[3] ==1 && sensors[4] ==1)
    {
        err = 0;
        base_speedl += 20;
        base_speedr +=20;
        line_found = true;
    }
    else if (sensors[3] ==1 || sensors[4] ==1){
        if (sensors[3] ==1) err = -7; else err = 7;
        base_speedl+=10;
        base_speedr+=10;
        line_found = true;
    }
    else if ((sensors[0] == 1 && sensors[1] == 1 && sensors[2] == 1) || (sensors[7] == 1 && sensors[6] == 1 && sensors[5] == 1 ))
    {
            if (sensors[0] == 1)
                err = -80;
            else
                err = 80;
    }
    else if (sensors[0] == 1 || sensors[7] == 1){
        if(sensors[0])
            if(line_found == true) err = -60; else err = 80;
        else
            if(line_found == true) err = 60; else err = -80;
        base_speedl-=15;
        base_speedr-=15;
        line_found = true;
        }

    else if(sensors[1]==1|| sensors[6]==1){
        if(sensors[1]) err = -40; else err =40;
        line_found = true;
    }

    else if (sensors[2] ==1 || sensors[5] ==1){
        if(sensors[2]) err = -25; else err =25;
        base_speedl;
        base_speedr;
        line_found = true;
    }
    else{
        if(last_err>0)
            err = -last_err;
        else if (last_err<0)
            err = last_err;
        else
            err = 0;
        line_found = false;
    }
    dErr = err - last_err;
    if(line_found) last_err = err;
    P = Kp*err;
    I += err;
    D = Kd*dErr;
    // out = (float)P + (float)I*Ki+ (float)D;
    out = (float)P;
    dl = (float)base_speedl + out;
    dr = (float)base_speedr - out;
    if (dl > max_speedl) dl = max_speedl;
    else if(dl<-max_speedl) dl = -max_speedl;
    if (dr > max_speedr) dr = max_speedr;
    else if(dr<-max_speedr) dr = -max_speedr;
    // Serial.print("*P" + String(err)+","+String(dl)+","+String(dr)+"#");
    motor(dl, dr);
}
void Aguro::stop_motor(){
    motor(0,0);
}

void Aguro::motor(int dl, int dr)
{
    float dist = s->read_ultrasonic();
    if (DEBUG){
        Serial.println("*D" + String(dist)+"#");
        }
    if (dist < 25)
    {
        dl = 0;
        dr = 0;
        // Serial.println("stopped, please move the obstacle!");
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
void (*resetFunc)(void) = 0;

void setup()
{
    pinMode(13,INPUT);
    pinMode(Relay,OUTPUT);
    digitalWrite(Relay,HIGH);
    // Serial.begin(115200);
    FastLED.addLeds<LED_TYPE, LED_PIN, COLOR_ORDER>(leds, NUM_LEDS);
    FastLED.setBrightness(BRIGHTNESS);
    aguro.init(true, &mysensor);
}

bool run = false;
int pb_counter=0;
void reset(){
    blink_led(5, 100, CRGB::Yellow);
    aguro.stop_motor();
    resetFunc();

}

void loop()
{
    
    if (digitalRead(PB) == HIGH)
    {
        run = !run;
        pb_counter++;
        if (pb_counter > 200)reset();
    }
    else
    {
        aguro.updateSensor();
        if(pb_counter>100){ //stop the program
            aguro.stop();
        }
    }
    aguro.stop_motor();
    blink_led(1, 100, CRGB::White);

    while (aguro.isStarted())
    {
        
        if (digitalRead(PB) == HIGH)
        {
            run = !run;
            pb_counter++;
            if (pb_counter > 200)reset();
        }
        else{
        
        if(pb_counter>100){ //stop the program
            // aguro.stop();
        }
    }
    if(run){
        aguro.traceLine(180);
        // Serial.println("tracing line");
    }
    else aguro.updateSensor();
    }
}
