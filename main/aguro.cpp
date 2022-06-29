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

    updateSensor();
    while (!sensors[3] && !sensors[4])
    {
        updateSensor();
        // if (sensors[SFL] || sensors[SFR])
        // {
        //     if (sensors[SFL])
        //     {
        //         motor(200, -200);
        //     }
        //     else
        //     {
        //         motor(-200, 200);
        //     }
        // }
        if (sensors[0] || sensors[7])
        {
            if (sensors[0])
            {
                motor(150, -150);
            }
            else
            {
                motor(-150, 150);
            }
        }
        else if (sensors[1] || sensors[6])
        {
            if (sensors[1])
            {
                motor(100, -100);
            }
            else
            {
                motor(-100, 100);
            }
        }
        else if (sensors[2] || sensors[5])
        {
            if (sensors[2])
            {
                motor(90, -90);
            }
            else
            {
                motor(-90, 90);
            }
        }
        else
        {
            motor(70, -70);
        }
    }
    motor(0, 0);
}

void Aguro::updateSensor(char sensor_id)
{
    if (sensor_id == -2)
    {
        sensors[SBL] = s->readlinebool(SBL);
        delayMicroseconds(10);
        sensors[SBR] = s->readlinebool(SBR);
        // Serial.println("Sensors: " + String(sensors[SBL]) + " " + String(sensors[SBR]));

        if (sensors[SBL] == 0)
        {
            leds[0] = CRGB(255, 0, 0);
            leds[1] = CRGB(255, 0, 0);
            leds[2] = CRGB(255, 0, 0);
            leds[3] = CRGB(255, 0, 0);
        }
        else
        {
            leds[0] = CRGB(0, 255, 0);
            leds[1] = CRGB(0, 255, 0);
            leds[2] = CRGB(0, 255, 0);
            leds[3] = CRGB(0, 255, 0);
        }
        if (sensors[SBR] == 0)
        {
            leds[4] = CRGB(255, 0, 0);
            leds[5] = CRGB(255, 0, 0);
            leds[6] = CRGB(255, 0, 0);
            leds[7] = CRGB(255, 0, 0);
        }
        else
        {
            leds[4] = CRGB(0, 255, 0);
            leds[5] = CRGB(0, 255, 0);
            leds[6] = CRGB(0, 255, 0);
            leds[7] = CRGB(0, 255, 0);
        }
        FastLED.show();
    }
    else if (sensor_id == -3)
    {
        sensors[SFL] = s->readlinebool(SFL);
        // Serial.println("Sensors: " + String(sensors[SFL]));
        sensors[SFR] = s->readlinebool(SFR);
        // Serial.println("Sensors: " + String(sensors[SFR]));
    }
    else if (sensor_id < 8 && sensor_id >= 0)
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
    else
    {
        // Serial.print("sensors : ");
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
            // delayMicroseconds(10);
            // Serial.print(sensors[i]);
            // Serial.print(" ");
        }
        // Serial.println();
        sensors[SBL] = s->readlinebool(SBL);
        // Serial.print(sensors[SBL]);
        // Serial.print("\t");
        sensors[SBR] = s->readlinebool(SBR);
        // Serial.print(sensors[SBR]);
        // Serial.print("\t");
        sensors[SFL] = s->readlinebool(SFL);
        // Serial.print(sensors[SFL]);
        // Serial.print("\t");
        sensors[SFR] = s->readlinebool(SFR);
        // Serial.print(sensors[SFR]);
        // Serial.print("\t");
        // Serial.println();
    }
}

// moving the robot
void Aguro::followUntil(char type, int speed, int stop_delay)
{
    bool found_garis = false;
    int pass_line = 10;
    // bool line[2] = {false, false};
    uint16_t last_time = 0;
    int SFL_last = 0;
    int SFR_last = 0;
    if (type == TJ)
    {
        for (int i = 0; i < 10; i++)
        {
            // traceLine(speed);
            updateSensor(-3);
            while (sensors[SFL] || sensors[SFR])
            {

                traceLine(speed);
                updateSensor(-3);
                last_time = millis();
                if (!sensors[SFL] && !sensors[SFR])
                {
                    found_garis = false;
                }
            }
            if (!found_garis)
                break;
        }
        for (int i = 0; i < pass_line; i++)
        {
            // traceLine(speed);
            // updateSensor();
            updateSensor(-3);
            while (!sensors[SFL] || !sensors[SFR])
            {
                if (sensors[SFL])
                {
                    SFL_last++;
                }
                else if (sensors[SFR])
                {
                    SFR_last++;
                }
                traceLine(speed);
                updateSensor(-3);
                // updateSensor();
                last_time = millis();

                if (sensors[SFL] && sensors[SFR] || SFL_last > 1 && SFR_last > 1)
                {

                    found_garis = true;
                }
            }
            if (found_garis)
                break;
        }
        for (int i = 0; i < 10; i++)
        {
            // traceLine(speed);
            updateSensor(-3);
            // updateSensor(-3);
            while (sensors[SFL] || sensors[SFR])
            {
                traceLine(speed);
                updateSensor(-3);
                // updateSensor(-3);

                last_time = millis();
                if (!sensors[SFL] && !sensors[SFR])
                {
                    found_garis = false;
                }
            }
            if (!found_garis)
                break;
        }
    }
    else if (type == FR)
    {
        for (int i = 0; i < 10; i++)
        {
            // traceLine(speed);
            updateSensor(-3);

            while (sensors[SFR])
            {
                traceLine(speed);
                updateSensor(-3);

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
            // traceLine(speed);
            updateSensor(-3);
            while (!sensors[SFR])
            {
                traceLine(speed);
                // updateSensor();
                updateSensor(-3);
                if (sensors[SFR])
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
            // traceLine(speed);
            updateSensor(-3);
            while (sensors[SFR])
            {
                traceLine(speed);
                // updateSensor();
                updateSensor(-3);

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
            // traceLine(speed);
            updateSensor(-3);
            while (sensors[SFL])
            {
                traceLine(speed);
                updateSensor(-3);
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
            // traceLine(speed);
            updateSensor(-3);
            while (!sensors[SFL])
            {
                traceLine(speed);
                updateSensor(-3);
                // updateSensor();

                if (sensors[SFL])
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
            updateSensor(-3);

            while (sensors[SFL])
            {
                traceLine(speed);
                updateSensor(-3);
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
    delay(stop_delay);
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
    int max_speedl = 240;
    int max_speedr = 240;
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
    // else if (sensors[SFL] == 1 || sensors[SFR] == 1)
    // {
    //     if (sensors[SFL])
    //         if (line_found)
    //             err = -100;
    //         else
    //             err = 80;
    //     else
    //     {
    //         if (line_found)
    //         {
    //             err = 100;
    //         }
    //         else
    //         {
    //             err = -80;
    //         }
    //     }
    //     line_found = true;
    // }
    else if (sensors[0] == 1 || sensors[7] == 1)
    {
        if (sensors[0])
            if (line_found)
                err = -70;
            else
                err = 60;
        else if (line_found)
            err = 70;
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

    if (err < 40 && err >= 0 || err > -40 && err <= 0)
    {
        motor(dl, dr + 25);
    }
    else
    {
        motor(dl - 25, dr);
    }
    //    delay(10);
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
        if (sensors[4] || sensors[3])
        {
            motor(0, 0);
            return;
        }
        motor(make_safe(speed), make_safe(0));
    }
}
void Aguro::right_auto()
{
    int err = 0, last_err = 0;
    int base_speed = 110;
    int max_speed = 160;
    uint16_t start_time = millis();
    while (true && millis() - start_time < 5000)
    {
        updateSensor();
        if (sensors[0] || sensors[7])
        {
            err = 1;
        }
        else if (sensors[1] || sensors[6])
        {
            err = 2;
        }
        else if (sensors[2] || sensors[5])
        {
            err = 3;
        }
        else if (sensors[3] || sensors[4])
        {
            err = 4;
        }
        else
        {
            base_speed = 125;
            break;
        }
        if (err == last_err)
        {
            base_speed += 5;
        }
        // Serial.println(base_speed);
        last_err = err;
        motor(constrain(base_speed, 0, max_speed), 0);
    }
    // Serial.println("keluar");
    // motor(0, 0);
    motor(constrain(base_speed, 0, max_speed), 0);
    updateSensor();
    while (!sensors[5] || !sensors[4])
    {
        updateSensor(5);
        updateSensor(4);
    }
    motor(0, 0);
    // Serial.println("finish");
}
void Aguro::left_auto()
{
    int err = 0, last_err = 0;
    int base_speed = 100;
    int max_speed = 180;
    uint16_t start_time = millis();
    while (true && millis() - start_time < 5000)
    {
        updateSensor();
        if (sensors[0] || sensors[7])
        {
            err = 1;
        }
        else if (sensors[1] || sensors[6])
        {
            err = 2;
        }
        else if (sensors[2] || sensors[5])
        {
            err = 3;
        }
        else if (sensors[3] || sensors[4])
        {
            err = 4;
        }
        else
        {
            base_speed = 135;
            break;
        }
        if (err == last_err)
        {
            base_speed += 5;
        }
        last_err = err;
        motor(0, constrain(base_speed, 0, max_speed));
    }

    // Serial.println("keluar");
    // motor(0, 0);
    motor(0, constrain(base_speed, 0, max_speed));
    updateSensor();
    while (!sensors[3] || !sensors[2])
    {
        updateSensor(2);
        updateSensor(3);
    }
    motor(0, 0);
    // Serial.println("finish");
}

void Aguro::left(int speed, int time)
{
    uint32_t time_start = millis();
    while (millis() - time_start < time)
    {
        if (sensors[3] || sensors[4])
        {
            motor(0, 0);
            return;
        }
        motor(-make_safe(0), make_safe(speed));
    }

    time_start = millis();
    while (millis() - time_start < 5000)
    {
        updateSensor();
        if (sensors[3] || sensors[4])
        {
            break;
        }
        else
        {
            motor(make_safe(0), make_safe(speed - 30));
        }
    }
    motor(0, 0);
}
void Aguro::maju(int speed, int time)
{
    uint32_t time_start = millis();
    while (millis() - time_start < time)
    {
        traceLine(speed);
    }
    motor(0, 0);
}

void Aguro::mundur(int speed, int time, bool check_switch)
{
    uint32_t time_start = millis();
    float err = 0;
    float gain = 10;
    // int d_speed = 30;
    bool initial_condition = true;
    while (millis() - time_start < time)
    {
        updateSensor(-2);

        if (sensors[SBR])
        {
            // motor(-170, -155);
            err = 1.1;
            mundur_flag = false;
            initial_condition = false;
        }
        else if (sensors[SBL])
        {
            // motor(-155, -170);
            err = -1.1;
            mundur_flag = true;
            initial_condition = false;
        }
        else
        {
            if (initial_condition)
            {
                err = 0;
            }
            else if (!mundur_flag)
            {
                err = 1;
                initial_condition = false;
            }
            else
            {
                err = -1;
                initial_condition = false;
            }
        }

        if (check_switch)
        {
            if (digitalRead(LimitSwitch))
            {
                motor(0, 0);
                break;
            }
        }
        int dl = speed + err * gain;
        int dr = speed - err * gain;
        motor(-(dl - 25), -(dr + 22));
        // centering();
    }
    motor(0, 0);
}

void Aguro::motor(int dl, int dr)
{
    float dist = s->read_ultrasonic();
    // Serial.println("dist : " + String(dist));
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