#include "engine.hpp"

void engine_init()
{
}

short unsigned int engine_getAngle(struct Step* curStep, short unsigned int time, enum Servos servo)
{
    struct Trajectory *tr = curStep->servoTrajectory + servo;

    if (tr->size == 0)
	    return 850;

    //search for current time in trajectory
    int i;
    for(i = 0; i < tr->size; i++)
    {
	if(tr->data[i].time >= time)
	{
	    break;
	}
    }

    //don't interpolate past the trajectory
    if(i == tr->size)
	return tr->data[i- 1].angle;
    
    //if time is correct, or it is the first angle in the 
    //trajectory we don't interpolate
    if(tr->data[i].time == time || i == 0)
	return tr->data[i].angle;

    struct AngleWithTime *prev = &(tr->data[i-1]);
    struct AngleWithTime *cur = &(tr->data[i]);
    
    //interpolate
    return prev->angle + ((int) (cur->angle - prev->angle)) * (time - prev->time) / (cur->time - prev->time);
}
