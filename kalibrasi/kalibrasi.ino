#include "sensor.h"

Sensor mysensor;
void setup(){
    pinMode(3, INPUT_PULLUP);
    Serial.begin(115200);
    FastLED.addLeds<LED_TYPE, LED_PIN, COLOR_ORDER>(leds, NUM_LEDS);
    FastLED.setBrightness(BRIGHTNESS);
    mysensor.init(true);
    mysensor.calibrateLine();   
}
void loop(){

}
