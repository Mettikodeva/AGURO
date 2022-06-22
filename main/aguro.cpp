#include "aguro.h"

bool Aguro::isStarted()
{
    return this->start;
}

void Aguro::stop()
{
    this->start = false;
    motor(0, 0);
    blink_led(1, 100, CRGB(0, 200, 200));
}

void Aguro::_start()
{
    this->start = true;
    this->updateSensor();
}

void Aguro::setPID(int P, int I, int D)
{
    this->Kp = P;
    this->Ki = I;
    this->Kd = D;
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
    digitalWrite(Relay, HIGH);
    if (sensor != NULL)
    {
        this->s = sensor;
        this->s->init(debug);
    }
    else
    {
        s = new Sensor();
        s->init(debug);
    }
    this->DEBUG = debug;
}

void Aguro::centering()
{
    float dl, dr, out;
    static signed int err = 0;
    float P, I, D;
    static signed int dErr, last_err;
    int max_speedl = 150;
    int max_speedr = 150;
    updateSensor();
    while (!(sensors[3] && sensors[4]))
    {
        updateSensor();
        if (sensors[SFL] || sensors[SFR])
        {
            if (sensors[SFL])
                err = -200;
            else
                err = 200;
        }
        else if (sensors[0] || sensors[7])
        {
            if (sensors[0])
                err = -100;
            else
                err = 100;
        }
        else if (sensors[1] || sensors[6])
        {
            if (sensors[1])
                err = -50;
            else
                err = 50;
        }
        else if (sensors[2] || sensors[5])
        {
            if (sensors[2])
                err = -10;
            else
                err = 10;
        }
        else if (sensors[3] || sensors[4])
        {
            if (sensors[3])
                err = -5;
            else
                err = 5;
        }
        else
        {
            err = last_err;
        }
        dErr = err - last_err;
        last_err = err;
        P = err * 1;
        I = I * 0.0001;
        D = D * 0.5;
        out = P + I + D;
        if (out > 0)
        {
            dl = out;
            dr = out;
        }
        else
        {
            dl = -out;
            dr = -out;
        }
        if (dl > max_speedl)
            dl = max_speedl;
        if (dr > max_speedr)
            dr = max_speedr;
        if (dl < -max_speedl)
            dl = -max_speedl;
        if (dr < -max_speedr)
            dr = -max_speedr;
        // Serial.println(err);
        motor(dl, dr);
    }
}

void Aguro::updateSensor(char sensor_id)
{
    if (sensor_id == -2)
    {
        sensors[SBL] = s->readlinebool(SBL);
        sensors[SBR] = s->readlinebool(SBR);
        if (sensors[SBL] == 0)
        {
            leds[0] = CRGB(255, 0, 0);
            leds[1] = CRGB(255, 0, 0);
            leds[2] = CRGB(255, 0, 0);
            leds[3] = CRGB(255, 0, 0);
            FastLED.show();
        }
        else
        {
            leds[0] = CRGB(0, 255, 0);
            leds[1] = CRGB(0, 255, 0);
            leds[2] = CRGB(0, 255, 0);
            leds[3] = CRGB(0, 255, 0);
            FastLED.show();
        }   
        if (sensors[SBR] == 0)
        {
            leds[4] = CRGB(255, 0, 0);
            leds[5] = CRGB(255, 0, 0);
            leds[6] = CRGB(255, 0, 0);
            leds[7] = CRGB(255, 0, 0);
            FastLED.show();
        }
        else
        {
            leds[4] = CRGB(0, 255, 0);
            leds[5] = CRGB(0, 255, 0);
            leds[6] = CRGB(0, 255, 0);
            leds[7] = CRGB(0, 255, 0);
            FastLED.show();
        }   
    }
    else if (sensor_id == -3)
    {
        sensors[SFL] = s->readlinebool(SFL);
        sensors[SFR] = s->readlinebool(SFR);
        
    }
    else if (sensor_id <8&&sensor_id >= 0)
    {
        sensors[sensor_id] = s->readlinebool(sensor_id);
        if (sensors[sensor_id] == 0)
        {
            leds[sensor_id] = CRGB(255, 0, 0);
            FastLED.show();
        }
        else
        {
            leds[sensor_id] = CRGB(0, 255, 0);
            FastLED.show();
        }
        
    }
    else{
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
        delayMicroseconds(10);
        
    }
    sensors[SBL] = s->readlinebool(SBL);
    sensors[SBR] = s->readlinebool(SBR);
    sensors[SFL] = s->readlinebool(SFL);
    sensors[SFR] = s->readlinebool(SFR);
    }
}

// moving the robot

void Aguro::followUntil(char type, int speed, int stop_delay)
{
    bool found_garis = false;
    int pass_line = 10;
    
    if (type == TJ)
    {
        for (int i = 0; i < 10; i++)
        {
            traceLine(speed);
            updateSensor(-3);
            while (sensors[SFL] || sensors[SFR])
            {
                traceLine(speed);
                updateSensor(-3);
                
                if (!sensors[SFL] && !sensors[SFR])
                    found_garis = false;
                else
                    found_garis = true;
            }
            if (!found_garis)
                break;
        }
        for (int i = 0; i < pass_line; i++)
        {
            traceLine(speed);
            // updateSensor();
            
            while (!sensors[SFL] || !sensors[SFR])
            {
                traceLine(speed);
                // updateSensor();
                
                if (sensors[SFL] && sensors[SFR])
                    found_garis = true;
                else
                    found_garis = false;
            }
            if (found_garis)
                break;
        }
        for (int i = 0; i < 10; i++)
        {
            traceLine(speed);
            // updateSensor(-3);
            
            while (sensors[SFL] || sensors[SFR])
            {
                traceLine(speed);
                // updateSensor(-3);
                
                if (!sensors[SFL] && !sensors[SFR])
                    found_garis = false;
                else
                    found_garis = true;
            }
            if (!found_garis)
                break;
        }
    }

    else if (type == FR)
    {
        for (int i = 0; i < 10; i++)
        {
            traceLine(speed);
            // updateSensor(-3);
            
            while (sensors[SFR])
            {
                traceLine(speed);
                // updateSensor();
                
                if (!sensors[SFR])
                    found_garis = false;
                else
                    found_garis = true;
            }
            if (!found_garis)
                break;
        }
        for (int i = 0; i < pass_line; i++)
        {
            // updateSensor();
            traceLine(speed);

            while (!sensors[SFR]&&sensors[SFL])
            {
                traceLine(speed);
                // updateSensor();
                
                if (sensors[SFR]&&!sensors[SFL])
                    found_garis = true;
                else
                    found_garis = false;
            }
            if (found_garis)
                break;
        }

        for (int i = 0; i < 10; i++)
        {
            // updateSensor();
            traceLine(speed);
            
            while (sensors[SFR])
            {
                traceLine(speed);
                // updateSensor();
                
                if (!sensors[SFR])
                    found_garis = false;
                else
                    found_garis = true;
            }
            if (!found_garis)
                break;
        }
    }
    else if (type == FL)
    {
        for (int i = 0; i < 10; i++)
        {
            // updateSensor();
            traceLine(speed);
            
            while (sensors[SFL])
            {
                traceLine(speed);
                // updateSensor();
                
                if (!sensors[SFL])
                    found_garis = false;
                else
                    found_garis = true;
            }
            if (!found_garis)
                break;
        }

        for (int i = 0; i < pass_line; i++)
        {
            // updateSensor();
            traceLine(speed);
            while (!sensors[SFL] && sensors[SFR])
            {
                traceLine(speed);
                // updateSensor();
                
                if (sensors[SFL] && !sensors[SFR])
                    found_garis = true;
                else
                    found_garis = false;
            }
            if (found_garis)
                break;
        }
        for (int i = 0; i < 10; i++)
        {
            // updateSensor();
            
            while (sensors[SFL])
            {
                traceLine(speed);
                // updateSensor();
                
                if (!sensors[SFL])
                    found_garis = false;
                else
                    found_garis = true;
            }
            if (!found_garis)
                break;
        }
    }
    delay(stop_delay)
    motor(0, 0);
}

void Aguro::traceLine(int speed, bool use_I)
{

    float dl, dr, out;
    static signed int err = 0;
    float P, I, D;
    static signed int dErr, last_err;
    int base_speedl = speed;
    int base_speedr = speed;
    int max_speedl = 230;
    int max_speedr = 230;
    bool flag_dir = false;

    updateSensor();
    //    0 1    2  (3 4)  5      6 7
    if (sensors[3] == 1 && sensors[4] == 1)
    {
        err = 0;
        line_found = true;
    }
    else if (sensors[3] == 1 || sensors[4] == 1)
    {
        if (sensors[3] == 1)
            err = -12;
        else
            err = 12;
        line_found = true;
    }
    else if ((sensors[0] == 1 && sensors[1] == 1) || (sensors[7] == 1 && sensors[6] == 1 ))
    {
        if (sensors[0] == 1)
            err = -80;
        else
            err = 80;
        line_found = true;
    }

    else if (sensors[0] == 1 || sensors[7] == 1)
    {
        if (sensors[0])
            if (line_found == true)
                err = -60;
            else
                err = 60;
        else
            if (line_found == true)
                err = 60;
            else
                err = -60;
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
        err = last_err;
        line_found = false;
    }

    dErr = err - last_err;
    if (line_found)
    {
        last_err = err;
    }
    P = Kp * err;
    D = Kd * dErr;

    if (I)
    {
        I += Ki * err;
        out = (float)P + (float)I + (float)D;
    }
    else
    {
        out = (float)P + (float)D;
    }

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
    
    // Serial.println("motor :");
    // Serial.print(dl);
    // Serial.print(" ");
    // Serial.print(dr);
    // Serial.println("err :"+String(err));
    motor(dl, dr);
    delay(10);

}

void Aguro::stop_motor()
{
    motor(0, 0);
}
void Aguro::right(int speed, int time)
{
    uint32_t time_start = millis();
    while (millis() - time_start < time)
    {
        motor(make_safe(speed), make_safe((int)speed / 5));
    }
    motor(0, 0);
    centering();
}

void Aguro::left(int speed, int time)
{
    uint32_t time_start = millis();
    while (millis() - time_start < time)
    {
        if (sensors[3] || sensors[2])
        {

            break;
        }
        motor(-make_safe(0), make_safe(speed + 10));
    }
    while (1)
    {
        updateSensor();
        if (sensors[3] || sensors[2])
        {

            break;
        }
        else
        {
            motor(-make_safe(0), make_safe(speed - 10));
        }
    }
    // motor(0, 0);
    // centering();
}

void Aguro::mundur(int speed, int time, bool check_switch)
{
    speed -=2f0;
    uint32_t time_start = millis();
    while (millis() - time_start < time)
    {
        updateSensor(-2);
        if (sensors[SBL]&&!sensors[SBR])
        {
            motor(-speed -10, -speed+25);
        }
        else if(sensors[SBR]&&!sensors[SBL])
        {
            motor(-speed+30, -speed -15);
        }
        else{
            motor(-speed, -speed-5 );
        }
        if (check_switch)
        {
            if (digitalRead(LimitSwitch))
            {
                motor(0, 0);
                break;
            }
        }
    }
    motor(0, 0);
    // centering();
}

void Aguro::motor(int dl, int dr)
{
    float dist = s->read_ultrasonic();
    if (dist < 30)
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

void Aguro::magnet(bool state)
{
    this->magnet_state = state;
    if (state)
    {
        digitalWrite(Relay, LOW);
    }
    else
    {
        digitalWrite(Relay, HIGH);
    }
}