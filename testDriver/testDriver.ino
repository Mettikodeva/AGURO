#define IN1 6  // left motor //PD6
#define IN2 7  // PD7
#define ENA 11 // PB2
#define IN3 8  // right motor //PB0
#define IN4 9  // PB1
#define ENB 10 // PB3
// #define buttonpin 2
// #define potentiopin A0

// void button_pushed();
void maju(int);
void mundur(int);

void setup()
{
    Serial.begin(115200);
    pinMode(IN1, OUTPUT);
    pinMode(IN2, OUTPUT);
    pinMode(IN3, OUTPUT);
    pinMode(IN4, OUTPUT);
    pinMode(ENA, OUTPUT);
    pinMode(ENB, OUTPUT);
    // attachInterrupt(buttonpin, button_pushed, LOW);
}
int speed = 100;
bool state = true;

void loop()
{
    // speed = map(analogRead(potentiopin), 0, 1023, 0, 255);
    if (state)
    {
        Serial.println("Maju");
        maju(speed);
    }
    else
    {
        Serial.println("Mundur");
        mundur(speed);
    }
    delay(1000);
    state = !state;
    speed+=20;
    if(speed>255){
      speed = 100;
    }
}

void maju(int speed)
{
    digitalWrite(IN1, LOW);
    digitalWrite(IN2, HIGH);
    analogWrite(ENA, speed);
    digitalWrite(IN3, LOW);
    digitalWrite(IN4, HIGH);
    analogWrite(ENB, speed);
}

void mundur(int speed)
{
    digitalWrite(IN1, HIGH);
    digitalWrite(IN2, LOW);
    analogWrite(ENA, speed);
    digitalWrite(IN3, HIGH);
    digitalWrite(IN4, LOW);
    analogWrite(ENB, speed);
}
// void button_pushed()
// {
//     state = !state;
// }
