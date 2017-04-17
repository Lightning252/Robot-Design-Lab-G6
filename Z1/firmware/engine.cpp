#include "engine.hpp"

//adds a new pattern
int add_pattern(int pattern_id){
	for (Pattern p_old : patterns){
		if (p_old.id==pattern_id){
			return -1;		
		}
	}
	Pattern p;
	p.id=pattern_id;
	patterns.emplace_back(p);
	return 0;
}

//adds a new trajectory
int add_trajectory(int pattern_id, int servo_id){
	for (Pattern p : patterns){
		if (p.id==pattern_id){
			for (Trajectory t_old : p.servos){
				if (t_old.id==servo_id){
					return -1;
				}
			}
			Trajectory t;
			t.id=servo_id;
			p.servos.emplace_back(t);	
			return 0;	
		}
	}
	return -2;	
}

//adds a new tuple
int add_tuple(int pattern_id, int servo_id, int time, int angle){
	for (Pattern p : atterns){
		if (p.id==pattern_id){
			for (Trajectory t : p.servos){
				if (t.id==servo_id){
					for (tuple<int,int> v_old : t.values) {
						if (get<0>(v_old)==time){
							return -1;
						}
					}
					tuple<int,int> v=(time,angle);
					t.values.emplace_back(v);
					return 0;
				}
			}
		}
	}
	return -2;
}

int get_angle(int pattern_id, int servo_id, int time){
	for (Pattern p : patterns){
		if (p.id==pattern_id){
			for (Trajectory t : p.servos){
				if (t.id==servo_id){
					for (tuple<int,int> v : t.values) {
						if (get<0>(v)==time){
							return get<1>(v);
						}
					}
					return get_estimate(t.values,time);
				}
			}
		}
	}
	return -2;
}

int get_estimate(vector<tuple<int,int>> values, int time){
	int before;
	int before_angle;
	int after;
	int after_angle; 
	for (tuple<int,int> v : values) {
		if (get<0>(v) > time){
			after = get<0>(v);
			after_angle = get<1>(v);
		}
	}
	for (tuple<int,int> v : values) {
		if (get<0>(v) < time){
			before = get<0>(v);
			before_angle = get<1>(v);
		}
	}
	for (tuple<int,int> v : values) {
		if ( (get<0>(v) > time) && (get<0>(v) < after) ){
			after = get<0>(v);
			after_angle = get<1>(v);
		}
	}
	for (tuple<int,int> v : values) {
		if ( (get<0>(v) < time) && (get<0>(v) > before) ){
			before = get<0>(v);
			before_angle = get<1>(v);
		}
	}
	int estimate = -1;
	estimate = (before_angle + after_angle) / 2;
	return estimate;
}

