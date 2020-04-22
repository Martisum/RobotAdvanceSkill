#include "PlaneGeometry.h"
#include "Skill.h"
#include "utils/maths.h"
#include <cmath>
extern "C"_declspec(dllexport) PlayerTask player_plan(const WorldModel* model, int robot_id);
using namespace PlaneGeometry;
double AngleMod(double act) { return act + 2 * PI; }
double AngleAmod(double act) { return act - 2 * PI; }
bool MaxComplement(float c[2], float b[2], float* result) {
	//memset(result, 0, 2 * sizeof(float));
	double originC[] = {c[0], c[1]};
	if(c[0] > c[1]) {
		double t = c[1];
		c[1] = c[0];
		c[0] = t;
	}
	if(b[0] > b[1]) {
		double t = b[1];
		b[1] = b[0];
		b[0] = t;
	}
	//has = true;
	if(b[0] < c[0]) {
		if(b[1] < c[1]) {
			if(b[1] > c[0]) {
				result[0] = b[1];
				result[1] = c[1];
			}
			else { result = c; }
		}
		else { return false; }
	}
	else if(b[0] > c[0] && b[1] > c[1]) {
		if(b[0] > c[1]) { result = c; }
		else {
			result[0] = c[0];
			result[1] = b[0];
		}
	}
	else {
		if(b[0] - c[0] > c[1] - b[1]) {
			result[0] = c[0];
			result[1] = b[0];
		}
		else {
			result[0] = b[1];
			result[1] = c[1];
		}
	}
	return true;
}
int c = 0;
PlayerTask player_plan(const WorldModel* model, int robot_id) {
	PlayerTask task;
	//球的坐标
	const point2f& ball_point = model->get_ball_pos();
	//我的坐标
	const double playerDir = model->get_our_player_dir(robot_id);
	const point2f& player_pos = model->get_our_player_pos(robot_id);
	const int OppGoalieID = model->get_opp_goalie();
	const point2f& EMGoalie_pos = model->get_opp_player_pos(OppGoalieID);
	const int size = model->robots_size();
	point2f goal_pos_left(300, -28);
	point2f goal_pos_right(300, 28);
	point2f goal(300, 0);
	float distToGoal = ball_point.dist(goal);
	float totalRange[] = {AngleMod((goal_pos_left - ball_point).angle()), AngleMod((goal_pos_right - ball_point).angle())};
	const bool* theirExists = model->get_opp_exist_id();
	bool has = true;
	for(int i = 0; i < 6; i++) {
		if(theirExists[i]) {
			point2f he = model->get_opp_player_pos(i);
			float dir = (he - ball_point).angle();
			float len = (he - ball_point).length();
			float dirR = atan2f(10, len);
			float cantShotRange[2] = {AngleMod(dir + dirR), AngleMod(dir - dirR)};
			has = MaxComplement(totalRange, cantShotRange, totalRange);
			if(!has) break;
		}
	}
	float finalDir;
	bool chip = false;
	if(has) {
		point2f p = ball_point;
		point2f p2 = ball_point + Maths::vector2polar(400, AngleAmod(totalRange[0]));
		point2f p3 = ball_point + Maths::vector2polar(400, AngleAmod(totalRange[1]));
	}
	if(has && fabs(totalRange[0] - totalRange[1]) > PI / 80) {
		finalDir = (AngleAmod(totalRange[0]) + AngleAmod(totalRange[1])) / 2;
		chip = false;
	}
	else {
		if(ball_point.y > 0) { finalDir = (point2f(300, -25) - ball_point).angle(); }
		else { finalDir = (point2f(300, 25) - ball_point).angle(); }
		chip = true;
	}
	task.orientate = finalDir;
	task.target_pos = ball_point;
	if((ball_point - player_pos).length() < size + 1.5) {
		if(fabs(model->get_our_player_dir(robot_id) - finalDir) < PI / 15) {
			task.target_pos = ball_point + Maths::vector2polar(ROBOT_HEAD + BALL_SIZE / 2, (ball_point - player_pos).angle());
			c++;
			if(c > 6) {
				if(chip) {
					task.kickPower = fmin(distToGoal / 10, 127);
					task.needKick = true;
					task.isChipKick = true;
					task.chipKickPower = fmin(distToGoal / 10, 127);
					c = 0;
				}
				else {
					task.target_pos = player_pos;
					if(c > 12) {
						task.isChipKick = false;
						task.needKick = true;
						task.kickPower = 127;
						c = 0;
					}
				}
			}
		}
	}
	task.needCb = true;
	return task;
}
