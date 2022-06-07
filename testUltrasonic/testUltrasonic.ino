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
    digitalWrite(trig, LOW);
    delayMicroseconds(2);
    digitalWrite(trig, HIGH);
    delayMicroseconds(10);
    digitalWrite(trig, LOW);
    float duration = pulseIn(echo, HIGH);
    float distance = duration / (29.1*2);
    Serial.print("Distance: ");
    Serial.println(distance);
}
