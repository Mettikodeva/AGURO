#ifndef sensor_h
#define sensor_h
#include "Arduino.h"
// sensors pin without mux or i2c  I/O expander

#define sp1 A0
#define sp2 A1
#define sp3 A2
#define sp4 A3
#define sp5 A4
#define sp6 A5
#define sp7 A6
#define sp8 A7

// ultrasonic pin
#define Trig 5 // PD5 //output
#define Echo 4 // PD4 //input

class Sensor
{
    /*
    TODO:
    1. save calibration to eeprom
    2. read calibration from eeprom
    3. add display function
    */
private:
    bool DEBUG;
    int sensors[8];
    // to save the calibration
    int max_line[8];
    int min_line[8];
    bool calibrated = false;
    bool line_high = false;

public:
    bool is_line_high();
    void init(bool debug = true);
    // to calibrate the line sensor for readlinebool
    void calibrateLine();
    char readsensorline();
    // to read the line sensor, SENSOR SHOULD BE CALIBRATED!
    //  1 = LINE FOUND
    //  0 = LINE NOT FOUND
    bool readlinebool(int index);
    // read ultrasonic and return in cm
    float read_ultrasonic();
};
#endif
