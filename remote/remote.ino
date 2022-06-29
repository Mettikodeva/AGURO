#include <stdlib.h>
#include <stdio.h>

#define IN1 10 // left motor //PD6
#define IN2 7  // PD7
#define ENA 11 // PB2

#define IN3 8 // right motor //PB0
#define IN4 9 // PB1
#define ENB 6 // PB3
#define MAJU 'w'
#define KIRI 'a'
#define KANAN 'd'
#define MUNDUR 's'

char checkKey()
{
    // const int BUFFER_SIZE = 3;
    // char buff[BUFFER_SIZE];
    int data = Serial.read();
    // ;
    // Serial.println(buff);
    while (data != '*')
    {

        if (Serial.read() == '*')
        {
            data = Serial.read();
            Serial.println(data);
            Serial.flush();

            return data;
        }
        else
        {
            return -1;
        }
    }
}

void motor(int dl, int dr)
{
    if (dl < 0)
    {
        digitalWrite(IN1, HIGH);
        digitalWrite(IN2, LOW);
        analogWrite(ENA, -dl);
    }
    else
    {
        digitalWrite(IN1, LOW);
        digitalWrite(IN2, HIGH);
        analogWrite(ENA, dl);
    }
    if (dr < 0)
    {
        digitalWrite(IN3, HIGH);
        digitalWrite(IN4, LOW);
        analogWrite(ENB, -dr);
    }
    else
    {
        digitalWrite(IN3, LOW);
        digitalWrite(IN4, HIGH);
        analogWrite(ENB, dr);
    }
}

void setup()
{
    Serial.begin(9600);
    Serial.println("Aguro");
    pinMode(IN1, OUTPUT);
    pinMode(IN2, OUTPUT);
    pinMode(ENA, OUTPUT);
    pinMode(IN3, OUTPUT);
    pinMode(IN4, OUTPUT);
    pinMode(ENB, OUTPUT);
}
void loop()
{
    char key = checkKey();
    switch (key)
    {
    case 'w':
        motor(120, 150);
        break;

    case 'a':
        motor(0, 130);
        break;

    case 'd':
        motor(100, 0);
        break;

    case 's':
        motor(-100, -135);
        break;

    case 'q':
        motor(80, 150);
        break;

    case 'e':
        motor(120, 100);
        break;

    default:
        break;
    }
}
