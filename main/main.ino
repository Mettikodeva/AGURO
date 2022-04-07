#include "sensor.h"
#include "aguro.h"

Aguro aguro;
Sensor mysensor;
void setup(){
    Serial.begin(115200);
    aguro.init(true, &mysensor);
}

void loop(){
    if(aguro.isStarted()){
        aguro.traceLine();
    }
}
