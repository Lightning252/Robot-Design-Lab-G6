#include "engine.hpp"

Pattern patterns[4];

//adds a new tuple
int add_tuple(int pattern_id, int servo_id, int time, int angle){
	if(time==0){
		patterns[pattern_id].servos[servo_id].values[0][0]=0;
		patterns[pattern_id].servos[servo_id].values[0][1]=angle;	
		return 0;
	}	
	for (int i=1; i<10; i++){
		if(patterns[pattern_id].servos[servo_id].values[i][0]==time){
			return -1;
		} else if(patterns[pattern_id].servos[servo_id].values[i][0]==0){
			patterns[pattern_id].servos[servo_id].values[i][0]=time;
			patterns[pattern_id].servos[servo_id].values[i][1]=angle;	
			return 0;
		}
	}
	return -2;
}

//looks up at what angle the servo should be at the given time
int get_angle(int pattern_id, int servo_id, int time){
	Trajectory t=patterns[pattern_id].servos[servo_id];
	for (int i=0; i<10; i++){
		if(t.values[i][0]==time){
			return t.values[i][1];
		}
	}
	return get_estimate(t,time);
}

//uses linear interpolation to estimate an angle
int get_estimate(Trajectory t, int time){
	double before=0;
	double before_angle=t.values[0][1];
	double after;
	double after_angle;
	for (int i=0; i<10; i++) {
		if (t.values[i][0] > time){
			after = t.values[i][0];
			after_angle = t.values[i][1];
			break;
		}
	}
	for (int i=0; i<10; i++) {
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



