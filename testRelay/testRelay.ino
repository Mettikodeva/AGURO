#define buttonpin 2
#define relaypin 3
bool state = false;

void button_pushed()
{
    state = !state;
}
void setup()
{
    pinMode(relaypin, OUTPUT);
    pinMode(buttonpin, INPUT);
    attachInterrupt(buttonpin, button_pushed, LOW);
}
void loop()
{
    if (state))
        {
            state = !state;
            digitalWrite(relaypin, HIGH);
        }
    else
    {
        digitalWrite(relaypin, LOW);
    }
}