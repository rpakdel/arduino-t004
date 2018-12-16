#include <IRLibDecodeBase.h>
#include <IRLib_P01_NEC.h>
#include <IRLibCombo.h>
#include <IRLibRecv.h> 
#include <../shared/Joy.h>
#include "motor.h"

int motorPins[2][2] = {
    { LMOTOR_1, LMOTOR_2 },
    { RMOTOR_1, RMOTOR_2 }
};

#define IR_RECV_PIN 4
IRrecv irrecv(IR_RECV_PIN);
uint16_t irbuffer[RECV_BUF_LENGTH];
IRdecode decoder;

#define MOTOR_SPEED_SCALE_SLOW 0.1f
#define MOTOR_SPEED_SCALE_MED 0.3f
#define MOTOR_SPEED_SCALE_FAST 0.5f
float currentMotorSpeedScale = MOTOR_SPEED_SCALE_SLOW;

void motor(MotorId motor, MotorDirection dir)
{
    int pin1 = motorPins[motor][0];
    int pin2 = motorPins[motor][1];

    switch (dir)
    {
    case MotorForward:
        analogWrite(pin1, 255);
        analogWrite(pin2, 0);
        break;

    case MotorBackward:
        analogWrite(pin1, 0);
        analogWrite(pin2, 255);
        break;

    case MotorStop:
        analogWrite(pin1, 0);
        analogWrite(pin2, 0);
        break;

    
    default:
        break;
    }
}

void motor(MotorId motor, float speed)
{ 
    speed = speed * currentMotorSpeedScale;
    MotorDirection dir = MotorStop;
    if (speed < 0)
    {
        speed = abs(speed) + MOTOR_PWM_DEADBAND;
        dir = MotorBackward;
    }
    else if (speed > 0)
    {
        speed += MOTOR_PWM_DEADBAND;
        dir = MotorForward;
    }

    if (speed > 1.0)
    {
        speed = 1.0;
    }

    int pwm = 255 - (int)(speed * 255.0f);

    int pin1 = motorPins[motor][0];
    int pin2 = motorPins[motor][1];
  
    switch (dir)
    {
        case MotorForward:
            analogWrite(pin1, 255);
            analogWrite(pin2, pwm);
            break;

        case MotorBackward:
            analogWrite(pin1, pwm);
            analogWrite(pin2, 255);
            break;

        case MotorStop:
            analogWrite(pin1, 255);
            analogWrite(pin2, 255);
            break;
    }
}

void setup()
{
    Serial.begin(9600);
    irrecv.enableAutoResume(irbuffer);
    irrecv.enableIRIn();

    pinMode(RMOTOR_1, OUTPUT);
    pinMode(RMOTOR_2, OUTPUT);

    pinMode(LMOTOR_1, OUTPUT);
    pinMode(LMOTOR_2, OUTPUT);

    pinMode(LED_BUILTIN, OUTPUT);
    //motor(LeftMotor, MotorStop);
    //motor(RightMotor, MotorStop);
    Serial.println("Motor ready");
}

float getLogScale(int8_t v)
{
    if (v == 0)
    {
        return 0;
    }
    else if (v < 0)
    {
        return -log10(-v + 1);
    }

    return log10(v + 1);
}

float getExpScale(int8_t v)
{
    // fraction 0-1
    float f = v / 7.0;

    if (v < 0)
    {
        return -1 * f * f;
    }

    return f * f;
}

float getLinearScale(int8_t v)
{
    // fraction 0-1
    return v / 7.0;
}

int clamp(int jinput)
{
    if (jinput > 7)
    {
        jinput = 7;
    }
    if (jinput < -7)
    {
        jinput = -7;
    }
    return jinput;
}

void handleRemoteCode(uint32_t h)
{
    Joy j = getJoyFromHex(h);
    printJoy(j, Serial);
    float angleRadians = atan2(j.y, j.x);
    float angleDegrees = angleRadians * 180 / 3.1415;
    Serial.print(" angle = ");
    Serial.print(angleDegrees);
    
    

    if (j.button == 1)
    {
        motor(LeftMotor, MotorStop);
        motor(RightMotor, MotorStop);
        if (currentMotorSpeedScale == MOTOR_SPEED_SCALE_SLOW)
        {
            currentMotorSpeedScale = MOTOR_SPEED_SCALE_MED;
            digitalWrite(LED_BUILTIN, HIGH);
        }
        else
        {
            currentMotorSpeedScale = MOTOR_SPEED_SCALE_SLOW;
            digitalWrite(LED_BUILTIN, LOW);
        }
    }
    else
    {
        int left = 0;
        int right = 0;

        if (j.y < 0)
        {
            left = clamp(j.y - j.x / 2.0f);
            right = clamp(j.y + j.x / 2.0f);
        }
        else
        {
            left = clamp(j.y + j.x / 2.0f);
            right = clamp(j.y - j.x / 2.0f);
        }

        Serial.print(" left = ");
        Serial.print(left);

        Serial.print(" right = ");
        Serial.print(right);

        

        float speedLeft = getExpScale(left);
        float speedRight = getExpScale(right);

        Serial.print(" speedleft = ");
        Serial.print(speedLeft);

        Serial.print(" speedright = ");
        Serial.print(speedRight);

        Serial.println();
        
        motor(LeftMotor, speedLeft);
        motor(RightMotor, speedRight);
    }    
}

float speed = 0;
int dir = MotorForward;

void testMotors()
{
    motor(RightMotor, speed);
    motor(LeftMotor, speed);


    switch (dir)

    {
    case MotorForward:
        speed += 0.1;
        break;

    case MotorBackward:
        speed -= 0.1;
        break;
    default:
        break;
    }

    if (speed > 1.0)
    {
        speed = 0;
        dir = MotorBackward;
    }
    else if (speed < -1.0)
    {
        speed = 0;
        dir = MotorForward;
    }
    delay(1000);
}

void loop()
{
    if (irrecv.getResults()) 
    {
        decoder.decode();
        handleRemoteCode(decoder.value);
        irrecv.enableIRIn();
    }
}
