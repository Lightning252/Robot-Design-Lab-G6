#include "engine.hpp"

Pattern patterns[4];

//adds a new pattern
int add_pattern(int pattern_id){
	Pattern p;
	patterns[pattern_id]=p;
	return 0;
}

//adds a new trajectory
int add_trajectory(int pattern_id, int servo_id){	
	Trajectory t;
	Pattern p_new=patterns[pattern_id];
	p_new.servos[servo_id]=t;
	patterns[pattern_id]=p_new;	
	return 0;		
}

//adds a new tuple
int add_tuple(int pattern_id, int servo_id, int time, int angle){
	if(time==0){
		Trajectory t;
		t=patterns[pattern_id].servos[servo_id];
		t.values[0][0]=0;
		t.values[0][1]=angle;
		Pattern p_new=patterns[pattern_id];
		p_new.servos[servo_id]=t;
		patterns[pattern_id]=p_new;	
		return 0;
	}	
	for (int i=1; i<10; i++){
		if(patterns[pattern_id].servos[servo_id].values[i][0]==time){
			return -1;
		}
	}
	for (int i=1; i<10; i++){
		if(patterns[pattern_id].servos[servo_id].values[i][0]==0){
			Trajectory t;
			t=patterns[pattern_id].servos[servo_id];
			t.values[i][0]=time;
			t.values[i][1]=angle;
			Pattern p_new=patterns[pattern_id];
			p_new.servos[servo_id]=t;
			patterns[pattern_id]=p_new;	
			return 0;
		}
	}
	return -2;
}

//looks up at what angle the servo should be at the given time
int get_angle(int pattern_id, int servo_id, int time){
	Trajectory t;
	t=patterns[pattern_id].servos[servo_id];
	for (int i=0; i<10; i++){
		if(t.values[i][0]==time){
			return t.values[i][1];
		}
	}
	return get_estimate(t,time);

}

//uses linear interpolation to estimate an angle
int get_estimate(Trajectory t, int time){
	int before=0;
	int before_angle;
	int after;
	int after_angle;
    	int estimate;
	before_angle=t.values[0][1];
	for (int i=0; i<10; i++) {
		if (t.values[i][0] > time){
			after = t.values[i][0];
			after_angle = t.values[i][1];
		}
	}
	for (int i=0; i<10; i++) {
		if ((t.values[i][0] < time) && (t.values[i][0] > before) ){
			before = t.values[i][0];
			before_angle = t.values[i][1];
		}
	}
	for (int i=0; i<10; i++) {
		if ((t.values[i][0] > time) && (t.values[i][0] < after) ){
			after = t.values[i][0];
			after_angle = t.values[i][1];
		}
	}
	estimate = before_angle + ((after_angle - before_angle)/(after-before))*(time-before);
	return estimate;
}



