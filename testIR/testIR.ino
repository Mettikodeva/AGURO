#define S0 A1 // PC1
#define S1 A2 // PC2
#define S2 A3 // PC3
#define Z A0  // MUX DATA PIN //PC0
#define buttonpin 2
int counter = 0;

int mux_bit[8][3] = {
    {0, 0, 0},
    {0, 0, 1},
    {0, 1, 0},
    {0, 1, 1},
    {1, 0, 0},
    {1, 0, 1},
    {1, 1, 0},
    {1, 1, 1}};

void button_pushed()
{
    if (counter == 8)
    {
        counter = 0;
    }
    else
    {
        counter++;
    }
}

void setup()
{
    pinMode(S0, OUTPUT);
    pinMode(S1, OUTPUT);
    pinMode(S2, OUTPUT);
    pinMode(Z, INPUT);
    attachInterrupt(buttonpin, button_pushed, LOW); // input pullup
}
void loop()
{
    switch (counter)
    {
    case 0:
        Serial.print("sensor 1 : ");
        Serial.println(read_sensor(counter));

        break;
    case 1:
        Serial.print("sensor 2 : ");
        Serial.println(read_sensor(counter));
        break;

    case 2:
        Serial.print("sensor 3 : ");
        Serial.println(read_sensor(counter));
        break;
    case 3:
        Serial.print("sensor 4 : ");
        Serial.println(read_sensor(counter));
        break;
    case 4:
        Serial.print("sensor 5 : ");
        Serial.println(read_sensor(counter));
        break;
    case 5:
        Serial.print("sensor 6 : ");
        Serial.println(read_sensor(counter));
        break;
    case 6:
        Serial.print("sensor 7 : ");
        Serial.println(read_sensor(counter));
        break;
    case 7:
        Serial.print("sensor 8 : ");
        Serial.println(read_sensor(counter));
        break;

    case 8:
        Serial.println("ALL SENSOR");
        for (int i = 0; i < 8; i++)
        {
            Serial.print("\t%s", read_sensor(i));
        }
        Serial.println();
        break;

    default:
        break;
    }
}

int read_sensor(int i)
{
    mux_control(i);
    return digitalRead(Z);
}

void mux_control(int i)
{
    digitalWrite(S0, mux_bit[i][0]);
    digitalWrite(S1, mux_bit[i][1]);
    digitalWrite(S2, mux_bit[i][2]);
}