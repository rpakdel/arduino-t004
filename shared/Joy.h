#pragma once
#include <Arduino.h>

#define JOYBUTTON_PIN 2

typedef struct _JOY_
{
    int8_t pad;

    int8_t x;
    int8_t y;
    int8_t button;
} Joy;

void resetJoy(Joy& j);
Joy getJoy();
void printJoy(Joy& j, Print& p);
uint32_t getJoyAsHex(Joy& j);
Joy getJoyFromHex(uint32_t h);