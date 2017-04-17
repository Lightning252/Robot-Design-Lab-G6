#include <vector>
#include <tuple>

vector<Pattern> patterns;

class Pattern{
	public:	
		int id;
		vector<Trajectory> servos;
}

class Trajectory{
	public:
		int id;
		vector<tuple<int,int>> values;
}

int add_pattern(int pattern_id);
int add_trajectory(int pattern_id, int servo_id);
int add_tuple(int pattern_id, int servo_id, int time, int angle);
int get_angle(int pattern_id, int servo_id, int time);
int get_estimate(vector<tuple<int,int>> values, int time);
