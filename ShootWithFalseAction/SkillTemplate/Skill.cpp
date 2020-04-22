#include "utils/constants.h"
#include "utils/PlayerTask.h"
#include "utils/worldmodel.h"
#include "utils/maths.h"
#include "utils/vector.h"
using namespace Vector;
using namespace Maths;
extern "C"_declspec(dllexport) PlayerTask player_plan(const WorldModel* model, int robot_id);
const pair<point2f, point2f> GoalPoints{
	point2f(FIELD_LENGTH_H + GOAL_DEPTH, GOAL_WIDTH_H),
	point2f(FIELD_LENGTH_H + GOAL_DEPTH, -GOAL_WIDTH_H)
};
bool HadFalseAction = false;
PlayerTask player_plan(const WorldModel* model, int robot_id) {
	PlayerTask task;
	const point2f ballPos = model->get_ball_pos();
	const point2f thisPlayerPos = model->get_our_player_pos(robot_id);
	const double thisPlayerDir = model->get_our_player_dir(robot_id);
	const bool* oppPlayerExistId = model->get_opp_exist_id();
	vector<point2f> oppPlayerPoses;
	for(int i = 0; i < MAX_ROBOTS; i++) {
		if(oppPlayerExistId[i]) {
			point2f pos = model->get_opp_player_pos(i);
			if(pos.x > thisPlayerPos.x)oppPlayerPoses.push_back(pos);
		}
	}
	pair<double, double> minDistance{DBL_MAX,DBL_MAX};
	if(oppPlayerPoses.size() < 1) {
		if(thisPlayerPos.x > 0) minDistance.first = 0;
		else minDistance.second = 0;
	}
	else {
		for(const point2f& pos : oppPlayerPoses) {
			minDistance.first = fmin(minDistance.first, distance_to_line(ballPos, GoalPoints.first, pos));
			minDistance.second = fmin(minDistance.second, distance_to_line(ballPos, GoalPoints.second, pos));
		}
	}
	const point2f goalPoint = minDistance.first > minDistance.second ^ HadFalseAction ? GoalPoints.first : GoalPoints.second;
	task.target_pos = ballPos + vector2polar(8, (ballPos - goalPoint).angle());
	task.orientate = (goalPoint - ballPos).angle();
	if(!HadFalseAction) { if(fabs(anglemod(thisPlayerDir - (ballPos - goalPoint).angle())) < PI / 6) HadFalseAction = true; }
	else {
		if((ballPos - thisPlayerPos).length() < BALL_SIZE / 2 + MAX_ROBOT_SIZE + 5 && anglemod(
			thisPlayerDir - (ballPos - thisPlayerPos).angle()) < PI / 6) {
			task.target_pos = ballPos + vector2polar(FLT_MAX, (ballPos - thisPlayerPos).angle());
			task.kickPower = 127;
			task.needKick = true;
			task.isChipKick = false;
			HadFalseAction = false;
		}
	}
	task.needCb = true;
	return task;
}
