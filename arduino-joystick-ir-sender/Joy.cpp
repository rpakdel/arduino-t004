#include "Joy.h"

int leftx = 515;
int rightx = -508;
int centerx = 508;

int upy = -504;
int downy = 519;
int centery = 504;

int zeroValue(int v)
{
    if (abs(v) < 15)
    {
        return 0;
    }
    return v;
}

int getXRaw()
{
    // depends on joystick orientation
    return analogRead(A1);
}

int getYRaw()
{
    // depends on joystick orientation
    return analogRead(A0);
}

int8_t getX()
{
    int x = getXRaw();
    x = x - centerx;
    //Serial.print("XRAW ");
    //Serial.println(x);
    
    x = zeroValue(x);
    x = map(x, leftx, rightx, -7, 7);

    //Serial.print(" X ");
    //Serial.println(x);
    return x;
}

int8_t getY()
{
    int y = getYRaw();
    //Serial.print("YRAW ");
    //Serial.print(y);

    y = y - centery;
    //Serial.print(" YZERO ");
    //Serial.print(y);
    
    y = zeroValue(y);
    y = map(y, downy, upy, -7, 7);
    //Serial.print(" Y ");
    //Serial.println(y);
    return y;
}

int8_t getButton()
{
    return 1 - digitalRead(JOYBUTTON_PIN);
}

void resetJoy(Joy & j)
{
    j.pad = 0;

    j.x = 0;
    j.y = 0;
    j.button = 0;
}

Joy getJoy()
{
    Joy j;
    resetJoy(j);
    j.x = getX();
    j.y = getY();
    j.button = getButton();
    return j;
}

void printJoy(Joy& j, Print& p)
{
    p.print(F("x"));
    p.print(j.x, DEC);
    p.print(F(", y"));
    p.print(j.y, DEC);
    p.print(F(", b"));
    p.print(j.button, DEC);
}

uint32_t getJoyAsHex(Joy& j)
{
    return *(uint32_t*)(void*)&j;
}

Joy getJoyFromHex(uint32_t h)
{
    return *(Joy*)(void*)&h;
}
