#ifndef SERVO_HPP__
#define SERVO_HPP__

enum Servos {
    SERVO_FR_HIP = 0,
    SERVO_FR_KNEE,
    SERVO_BR_HIP,
    SERVO_BR_KNEE,
    SERVO_BL_HIP,
    SERVO_BL_KNEE,
    SERVO_FL_HIP,
    SERVO_FL_KNEE,
    SERVO_NUM_SERVOS,
};

/**
* Value is in Angle * 10
* 0 is O degrees
* 1700 is 170 degrees
*
* Values out of range will trigger an assertion
*/
void servo_setAngle(enum Servos servo, int value);

/**
 * Initialized the hardware for PWM generation. 
 * */
void servo_init();

void overruntest();

void cameraclock();

#endif
