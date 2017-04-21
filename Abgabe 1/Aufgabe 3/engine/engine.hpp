struct Trajectory{
        int values[10][2]={0};
};

struct Pattern{
	Trajectory servos[8];
};

int add_pattern(int pattern_id);
int add_trajectory(int pattern_id, int servo_id);
int add_tuple(int pattern_id, int servo_id, int time, int angle);
int get_angle(int pattern_id, int servo_id, int time);
int get_estimate(Trajectory t, int time);
