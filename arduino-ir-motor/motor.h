#pragma once

// right motor when viewed from top
#define RMOTOR_1 5
#define RMOTOR_2 6
// left motor when viewed from top
#define LMOTOR_1 10
#define LMOTOR_2 9

#define LMOTOR 0
#define RMOTOR 1

// minimum value to get motors moving
#define MOTOR_PWM_DEADBAND 0.15

enum MotorId {
    LeftMotor = 0,
    RightMotor = 1
};

enum MotorDirection {
    MotorBackward = -1,
    MotorStop = 0,
    MotorForward = 1,
};