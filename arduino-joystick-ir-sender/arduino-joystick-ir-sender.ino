#include <IRLib2.h>
#include <IRLibSendBase.h>    
#include <IRLib_P01_NEC.h>    
#include "Joy.h"

IRsend sender;

#define MAX_SEND_DELAY 5000 // every 5 seconds

void setup()
{
    Serial.begin(9600);
    pinMode(JOYBUTTON_PIN, INPUT_PULLUP);
    Serial.println(F("Sender Ready"));
}

void sendJoy(Joy& j)
{
    uint32_t joyData = getJoyAsHex(j);
    printJoy(j, Serial);
    //Serial.print(" ---> 0x");
    //Serial.print(joyData, HEX);
    //Serial.println();
    sender.send(NEC, joyData);
}

long prevSendMillis = 0;
bool hasJoyValue = false;
void loop()
{
    Joy j = getJoy();

    long m = millis();

    if (j.x != 0 || j.y != 0 || j.button != 0)
    {
        hasJoyValue = true;
        sendJoy(j);
        prevSendMillis = m;
    }
    else if (hasJoyValue)
    {
        hasJoyValue = false;
        sendJoy(j);
        prevSendMillis = m;
    }
    else
    {
        long diff = m - prevSendMillis;
        if (diff >= MAX_SEND_DELAY)
        {
            sendJoy(j);
            prevSendMillis = m;
        }
    }
    
    delay(100);
}
