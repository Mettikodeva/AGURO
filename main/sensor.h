#ifndef SENSOR_H
#define SENSOR_H
#include "definitions.h"

// sensors pin without mux or i2c  I/O expander

// #define sp1 A0
// #define sp2 A1
// #define sp3 A2
// #define sp4 A3
// #define sp5 A4
// #define sp6 A5
// #define sp7 A6
// #define sp8 A7
#define PushButton 3
#define MUXData A0
#define MUXPinA A1
#define MUXPinB A2
#define MUXPinC A3
#define SpinFL A4
#define SpinFR A5
#define SpinBL A6
#define SpinBR A7
#define SFL 8
#define SFR 9
#define SBL 10
#define SBR 11
// ultrasonic pin
#define Trig 5 // PD5 //output
#define Echo 4 // PD4 //input
static unsigned char MUX[8][3] = {
    {0, 0, 0},
    {0, 0, 1},
    {0, 1, 0},
    {0, 1, 1},
    {1, 0, 0},
    {1, 0, 1},
    {1, 1, 0},
    {1, 1, 1}};

class Sensor
{
    /*
    TODO:
    1. save calibration to eeprom  // MAX_LINE WRITTEN FROM INDEX 0-7(times sizeof(int))
    2. read calibration from eeprom // MIN_LINE WRITTEN FROM INDEX 8-15(times sizeof(int))
    */
private:
    bool DEBUG;
    int sensors[12];
    // to save the calibration
    int max_line[12];
    int min_line[12];
    bool calibrated = false;
    bool line_high = false;
    void save_calibration();
    void read_calibration();
    void setMux(int);

public:
    bool is_line_high();
    void init(bool debug = true);
    // to calibrate the line sensor for readlinebool
    void calibrateLine();
    // to read the line sensor, SENSOR SHOULD BE CALIBRATED!
    //  1 = LINE FOUND
    //  0 = LINE NOT FOUND
    bool readlinebool(int index);
    // read ultrasonic and return in cm
    float read_ultrasonic();
};
#endif
