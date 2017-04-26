#ifndef ENGINE_HPP__
#define ENGINE_HPP__

#define MAX_TRAJECTORY_SIZE 16
#define NUM_SERVOS 8

#include "servo.hpp"

struct AngleWithTime
{
    unsigned short angle;
    unsigned short time;
} __attribute__ ((packed)) __attribute__((__may_alias__));

struct Trajectory
{
    unsigned short size;
    struct AngleWithTime data[MAX_TRAJECTORY_SIZE];
} __attribute__ ((packed)) __attribute__((__may_alias__));

struct Step
{
    //the length of the trajectory in ms
    unsigned int length;
    struct Trajectory servoTrajectory[NUM_SERVOS];
} __attribute__ ((packed)) __attribute__((__may_alias__));

/**
 * Initializes internal data structures. 
 * */
void engine_init();

/**
 * This function returns the angle of the regested servo
 * at the requested time from the given step.
 * 
 * @arg curStep The step from which the servo position should be calculated
 * @arg time The time in ms at which the servo position should be calculated
 * @arg servo The id of the servo for which the position should be calculated
 *
 * @return The position of the Servo in degrees * 100
 * */
short unsigned int engine_getAngle(struct Step *curStep, short unsigned int time, enum Servos servo);

#endif
