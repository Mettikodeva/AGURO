#include "aguro.h"
void (*resetFunc)(void) = 0;

int Aguro::checkButton(){
    uint32_t time = 0;
        if(digitalRead(PushButton)){
            if(this->button_pressed){
                if (millis() - this->press_start > RESET_PRESS && this->button_flag == false)
				{
					this->button_flag = true;
                    blink_led(5,300,CRGB(230,00,0));
                    resetFunc();
				}
                else{
                    time = millis()-this->press_start;
                }
            }
            else{
                this->button_pressed = true;
                this->press_start = millis();
            }
            
        }
        else{
            this->button_pressed = false;
            this->button_flag = false;
            
        }

        if(time > LONG_PRESS){
            blink_led(5,300,CRGB(200,200,0));
            this->stop();

        }
        else if(time > MEDIUM_PRESS){
            blink_led(5,300,CRGB(150,250,0));

        }
        else if(time>SHORT_PRESS){
            blink_led(5,300,CRGB(0,250,0));
            
        } 
}

bool Aguro::isStarted()
{
    return this->start;
}
void Aguro::stop()
{
    this->start = false;
    motor(0,0);
    blink_led(1,100,CRGB(0,200,200));
}

void Aguro::_start(){
    this->start = true;
    this->updateSensor();
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
    pinMode(PB, INPUT);
    digitalWrite(Relay, HIGH);
    s = sensor;
    s->init(debug);
    this->DEBUG = debug;
    s->calibrateLine();
}

void Aguro::centering()
{
    updateSensor();
}

void Aguro::updateSensor()
{
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
}

// moving the robot
void Aguro::followUntil(char type, int speed)
{
    bool found_garis = false;
    int pass_line = 10;
    if (type == TJ)
    {
        for (int i = 0; i < 100; i++)
        {
            updateSensor();
            while (sensors[0] || sensors[7])
            {
                traceLine(speed);
                updateSensor();
                if (sensors[0] && sensors[7])
                    found_garis = true;
                else
                    found_garis = false;
            }
            if (found_garis)
                break;
        }
        for (int i = 0; i < pass_line; i++)
        {
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
        for (int i = 0; i < 10; i++)
        {
            updateSensor();
            while (sensors[0] || sensors[7])
            {
                updateSensor();
                motor(speed, speed);
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

    float dl, dr, out;
    signed char err = 0;
    static signed int P, D, dErr, last_err;
    float Kp = 2, Kd = 0;
    int base_speedl = speed;
    int base_speedr = speed;
    int max_speedl = 220;
    int max_speedr = 220;
    bool flag_dir = false;

    updateSensor();
    //    0 1  2  3 4  5  6 7
    if (after_turn > 0)
    {
        delay(10);
        centering();
        after_turn = 0;
    }

    // if ((sensors[0] == 1 && sensors[1] == 1 && sensors[2] == 1&&sensors[3]==1) || (sensors[7] == 1 && sensors[6] == 1 && sensors[5] == 1&&sensors[4]==1)){
    //     if(sensors[0] == 1){
    //         err = -120;
    //         // base_speedl -= 50;
    //     }
    //     else{
    //         // base_speedr -= 50;
    //         err = 120;
    //     }
    //     after_turn++;
    //     line_found = true;
    // }
    if (sensors[3] == 1 && sensors[4] == 1)
    {
        err = 0;
        line_found = true;
    }
    else if (sensors[3] == 1 || sensors[4] == 1)
    {
        if (sensors[3] == 1)
            err = -10;
        else
            err = 10;
        line_found = true;
    }
    else if ((sensors[0] == 1 && sensors[1] == 1 && sensors[2] == 1) || (sensors[7] == 1 && sensors[6] == 1 && sensors[5] == 1))
    {
        if (sensors[0] == 1)
            err = -100;
        else
            err = 100;
        line_found = true;
    }

    else if (sensors[0] == 1 || sensors[7] == 1)
    {
        if (sensors[0])
            if (line_found == true)
                err = -80;
            else
                err = 80;
        else if (line_found == true)
            err = 80;
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
        line_found = true;
    }

    else if (sensors[2] == 1 || sensors[5] == 1)
    {
        if (sensors[2])
            err = -20;
        else
            err = 20;
        base_speedl;
        base_speedr;
        line_found = true;
    }
    else
    {
        if (last_err > 0)
            err = -last_err;
        else if (last_err < 0)
            err = last_err;
        else
            err = 0;
        line_found = false;
    }

    dErr = err - last_err;
    if (line_found)
    {
        last_err = err;
    }
    P = Kp * err;
    // I += err;
    D = Kd * dErr;
    // out = (float)P + (float)I*Ki+ (float)D;
    out = (float)P + (float)D;
    dl = (float)base_speedl + out;
    dr = (float)base_speedr - out;
    if (dl > max_speedl)
        dl = max_speedl;
    else if (dl < -max_speedl)
        dl = -max_speedl;
    if (dr > max_speedr)
        dr = max_speedr;
    else if (dr < -max_speedr)
        dr = -max_speedr;
    // Serial.print("*P" + String(err)+","+String(dl)+","+String(dr)+"#");
    motor(dl, dr);
}

void Aguro::stop_motor()
{
    motor(0, 0);
}
void Aguro::right(int speed, int time){
    uint32_t time_start = millis();
    while(millis()-time_start< time){
        motor(make_safe(speed),-make_safe(speed));
    }
    motor(0,0);
    centering();
}

void Aguro::left(int speed, int time){
    uint32_t time_start = millis();
    while(millis()-time_start< time){
        motor(-make_safe(speed),make_safe(speed));
    }
    motor(0,0);
    centering();
}

void Aguro::mundur(int speed, int time){
    uint32_t time_start = millis();
    while(millis()-time_start< time){
        motor(-make_safe(speed),-make_safe(speed));
    }
    motor(0,0);
}

void Aguro::motor(int dl, int dr)
{
    float dist = s->read_ultrasonic();
    if (dist < 25)
    {
        dl = 0;
        dr = 0;
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