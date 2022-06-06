#include "aguro.h"
#include "sensor.h"
#include "protothreads.h"

Aguro aguro;
Sensor mysensor;
pt ptButton;


int Button(struct pt* pt, Aguro* aguro){
    PT_BEGIN(pt);
    for(;;){
        Serial.println("this is check button");
        aguro->checkButton();
        Serial.println("finish check button");
        PT_YIELD(pt);
    }
    PT_END(pt);
}

pt ptLoop;
int main_loop(struct pt* pt, Aguro* aguro){
    PT_BEGIN(pt);
    for(;;){
        Serial.println("this is main loop");
        aguro->updateSensor();
        aguro->traceLine(200);
        // delay(2000);
        PT_YIELD(pt);
    }
    PT_END(pt);
}

void setup()
{
    Serial.begin(115200);
    FastLED.addLeds<LED_TYPE, LED_PIN, COLOR_ORDER>(leds, NUM_LEDS);
    FastLED.setBrightness(BRIGHTNESS);
    aguro.init(true, &mysensor);
    PT_INIT(&ptButton);
    PT_INIT(&ptLoop);
}


void loop()
{
    PT_SCHEDULE(Button(&ptButton, &aguro));
    PT_SCHEDULE(main_loop(&ptLoop, &aguro));
}
