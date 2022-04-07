#define D1 A0
#define D2 A1
#define D3 A2
#define D4 A3
#define D5 A4
#define D6 A5
#define D7 A6
#define D8 A7


void setup()
{
    Serial.begin(115200);
    pinMode(D1, INPUT);
    pinMode(D2, INPUT);
    pinMode(D3, INPUT);
    pinMode(D4, INPUT);
    pinMode(D5, INPUT);
    pinMode(D6, INPUT);
    pinMode(D7, INPUT);
    pinMode(D8, INPUT);
    
}
void loop()
{
    Serial.print(analogRead(D1));
    Serial.print("\t");
    Serial.print(analogRead(D2));
    Serial.print("\t");
    Serial.print(analogRead(D3));
    Serial.print("\t");
    Serial.print(analogRead(D4));
    Serial.print("\t");
    Serial.print(analogRead(D5));
    Serial.print("\t");
    Serial.print(analogRead(D6));
    Serial.print("\t");
    Serial.print(analogRead(D7));
    Serial.print("\t");
    Serial.println(analogRead(D8));
    delay(10);
}
