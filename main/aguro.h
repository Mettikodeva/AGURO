#ifndef AGURO_H
#define AGURO_H
#include "sensor.h"

// Driver motor pin
#define IN1 6  // left motor //PD6
#define IN2 7  // PD7
#define ENA 11 // PB2
#define IN3 8  // right motor //PB0
#define IN4 9  // PB1
#define ENB 10 // PB3

#define Relay 13 // relay for controling electromagnet

// followUntil type
#define TJ 0 // T junction or 4 way
#define FR 1 // Front Right turn
#define FL 2 // Front Left turn

// #define LCD_ADDR 0x27
// I2C pin
// #define SDA A4
// #define SCL A5

// API for the robot methods
class Aguro
{
private:
    bool start = true;
    bool DEBUG = true;
    bool sensors[12];
    Sensor *s;
    bool line_found = false;
    bool garisKiri = false;
    bool garisKanan = false;
    bool magnet_state = false;
    float Kp = 1.53;
    float Kd = 0;
    float Ki = 0.0001;
    bool mundur_flag = true; // true if right sensor last seen

public:
    void updateSensor(char sensor_id = -1);
    // initialize all the pin mode and interrupt and communication if present
    void init(bool debug = true, Sensor *sensor = NULL);
    // just to follow closed loop line without junction
    void traceLine(int speed, bool use_I = false);
    void maju(int speed, int time);
    void followUntil(char type, int speed, int stop_delay = 0);
    void magnet(bool);
    void Aguro::setPID(int P, int I, int D);
    void motor(int dl, int dr);
    bool isStarted();
    void centering();
    void stop();
    void _start();
    void stop_motor();
    void left(int, int);
    void right(int, int);
    void mundur(int, int, bool check_switch = false);
};

#endif
