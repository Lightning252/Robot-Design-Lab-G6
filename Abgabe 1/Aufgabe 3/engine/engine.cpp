#include "engine.hpp"

//4 patterns, each having 8 trajectorys.
Pattern patterns[4];

//currently each trajectory can hold 10 tuples
int maxtuple = 10;

//adds a new tuple to a trajectory in a pattern
//wants a pattern id, a servo id and time (in ms) + angle of the tuple
//returns 0 on success
//returns -1 if pattern id, servo id or time are invalid
//returns -2 if there already is a tuple for that tiem

 int add_tuple(int pattern_id, int servo_id, int time, int angle){
	if(pattern_id<0||pattern_id>3||servo_id<0||servo_id>7||time<0){
		return -1;
	}
	if(time==0){
		patterns[pattern_id].servos[servo_id].values[0][0]=0;
		patterns[pattern_id].servos[servo_id].values[0][1]=angle;	
		return 0;
	}	
	for (int i=1; i<maxtuple; i++){
		if(patterns[pattern_id].servos[servo_id].values[i][0]==time){
			return -2;
		} else if(patterns[pattern_id].servos[servo_id].values[i][0]==0){
			patterns[pattern_id].servos[servo_id].values[i][0]=time;
			patterns[pattern_id].servos[servo_id].values[i][1]=angle;	
			return 0;
		}
	}
	return -1;
}

//looks up at what angle the servo should be at the given time
//wants a pattern id, a servo id and the time in ms
//returns the angle for the given time
int get_angle(int pattern_id, int servo_id, int time){
	if(pattern_id<0||pattern_id>3||servo_id<0||servo_id>7||time<0){
		return -1;
	}
	Trajectory t=patterns[pattern_id].servos[servo_id];
	for (int i=0; i<maxtuple; i++){
		if(t.values[i][0]==time){
			return t.values[i][1];
		}
	}
	return get_estimate(t,time);
}

//uses linear interpolation to estimate an angle
//wants a trajectory and a time in ms
//returns an estimated angle 
int get_estimate(Trajectory t, int time){
	double before=0;
	double before_angle=t.values[0][1];
	double after;
	double after_angle;
	for (int i=0; i<maxtuple; i++) {
		if (t.values[i][0] > time){
			after = t.values[i][0];
			after_angle = t.values[i][1];
			break;
		}
	}
	for (int i=0; i<maxtuple; i++) {
		if ((t.values[i][0] < time) && (t.values[i][0] > before) ){
			before = t.values[i][0];
			before_angle = t.values[i][1];
		} else if ((t.values[i][0] > time) && (t.values[i][0] < after) ){
			after = t.values[i][0];
			after_angle = t.values[i][1];
		}
	}
	double estimate_d = before_angle + ((after_angle - before_angle)/(after-before))*(time-before);
	int estimate = estimate_d;
	return estimate;
}



