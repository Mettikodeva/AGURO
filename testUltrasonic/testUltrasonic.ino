#define trig 5 // PD5 //output
#define echo 4 // PD4 //input

void setup()
{
    Serial.begin(9600);
    pinMode(trig, OUTPUT);
    pinMode(echo, INPUT);
}
void loop()
{
    digitalWrite(Trig, LOW);
    delayMicroseconds(2);
    digitalWrite(Trig, HIGH);
    delayMicroseconds(10);
    digitalWrite(Trig, LOW);
    float duration = pulseIn(echo, HIGH);
    float distance = duration / 0.0343;
    Serial.print("Distance: ");
    Serial.println(distance);
}
