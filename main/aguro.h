
#ifndef aguro_h
#define aguro_h

#include "Arduino.h"
#include "sensor.h"
#include <stdio.h>
#include "Wire.h"

// Driver motor pin
#define IN1 6  // left motor //PD6
#define IN2 7  // PD7
#define ENA 11 // PB2
#define IN3 8  // right motor //PB0
#define IN4 9  // PB1
#define ENB 10 // PB3

#define PB 13 // push button //PCINT0 //should be input pullup

#define Relay 2 // relay for controling electromagnet //PD3

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
    /*
    TODO :  add pid at traceLine
    */
private:
    bool start = true;
    bool DEBUG = true;
    bool sensors[8];
    Sensor *s;
    bool line_found = false;
    int after_turn = 0;
    // void buttonInterrupt();
    // bool button_pressed = false;
    // int last_err=0;
    // int I=0;
public:
    void updateSensor();
    // initialize all the pin mode and interrupt and communication if present
    void init(bool debug=true, Sensor *sensor = NULL);
    // just to follow closed loop line without junction
    void traceLine(int speed);
    void followUntil(char type, int speed);

    void motor(int dl, int dr);
    bool isStarted();
    void centering();
    void stop();
    void stop_motor();
};


int make_safe(int val);

#endif
