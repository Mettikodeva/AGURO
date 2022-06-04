#include "aguro.h"

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
    float dl, dr, out;
    signed char err = 0;
    static float I;
    static signed char P, D, dErr, last_err;
    float Kp = 3, Ki = 0.05, Kd = 2;
    int max_speedl = 250;
    int max_speedr = 250;
    int counter = 0;
    while (!sensors[3] && !sensors[4] && counter > 3)
    {
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
            counter += 2;
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
    static float I;
    static signed int P, D, dErr, last_err;
    float Kp = 2.2, Ki = 0, Kd = 0.5;
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
        // base_speedl += 20;
        // base_speedr +=20;
        line_found = true;
    }
    else if (sensors[3] == 1 || sensors[4] == 1)
    {
        if (sensors[3] == 1)
            err = -10;
        else
            err = 10;
        // base_speedl+=10;
        // base_speedr+=10;
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
        // base_speedl-=15;
        // base_speedr-=15;
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
    // D = Kd*dErr;
    // out = (float)P + (float)I*Ki+ (float)D;
    out = (float)P;
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

void Aguro::motor(int dl, int dr)
{
    float dist = s->read_ultrasonic();
    if (DEBUG)
    {
        Serial.println("*D" + String(dist) + "#");
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