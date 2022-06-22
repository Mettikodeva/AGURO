#include <EEPROM.h>

int EEPROMReadInt(int address)
{
    long byte1 = EEPROM.read(address);
    long byte2 = EEPROM.read(address + 1);
    return ((byte2 << 0) & 0xFF) + ((byte1 << 8) & 0xFFFF);
}
void setup()
{
    Serial.begin(9600);
}
void loop()
{
    Serial.println("HIGH");
    for(int i = 0; i<12; i++)
    {
        int value = EEPROMReadInt(i*sizeof(int));
        Serial.println("nilai" + String(i) + ": " + String(value));
    }
    Serial.println();
    Serial.println("LOW");
    for(int i = 12; i<24; i++)
    {
        int value = EEPROMReadInt(i*sizeof(int));
        Serial.println("nilai" + String(i) + ": " + String(value));
    }
    Serial.println();
}
