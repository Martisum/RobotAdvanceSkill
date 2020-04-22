#include "Skill.h"
#include "utils/constants.h"
#include "utils/maths.h"
extern "C"_declspec(dllexport) PlayerTask player_plan(const WorldModel* model, int robot_id);
using namespace Maths;
const pair<point2f, point2f> GoalPoints{
	point2f(FIELD_LENGTH_H + GOAL_DEPTH, GOAL_WIDTH_H - 5),
	point2f(FIELD_LENGTH_H + GOAL_DEPTH, -(GOAL_WIDTH_H - 5))
};
PlayerTask player_plan(const WorldModel* model, int robot_id) {
	PlayerTask task;
	const point2f ballPos = model->get_ball_pos();
	const point2f playerPos = model->get_our_player_pos(robot_id);
	//const bool* oppPlayerExistId = model->get_opp_exist_id();
	//vector<point2f> oppPlayerPoses;
	//for(int i = 0; i < MAX_ROBOTS; i++) {
	//	if(oppPlayerExistId[i]) {
	//		point2f pos = model->get_opp_player_pos(i);
	//		if(pos.x > ballPos.x)oppPlayerPoses.push_back(pos);
	//	}
	//}
	//pair<double, double> minDistance{DBL_MAX, DBL_MAX};
	//if(oppPlayerPoses.empty()) {
	//if(ballPos.y < 0) minDistance.first = 0;
	//else minDistance.second = 0;
	//}
	//else {
	//	for(const point2f& pos : oppPlayerPoses) {
	//		minDistance.first = fmin(minDistance.first, distance_to_line(ballPos, GoalPoints.first, pos));
	//		minDistance.second = fmin(minDistance.second, distance_to_line(ballPos, GoalPoints.second, pos));
	//	}
	//}
	//const point2f goalPoint = minDistance.first < minDistance.second ? GoalPoints.first : GoalPoints.second;
	//task.target_pos = ballPos + vector2polar(BALL_SIZE + ROBOT_HEAD, (ballPos - goalPoint).angle());
	//task.orientate = (ballPos - playerPos).angle();
	const point2f goalPoint = ballPos.y < 0 ? GoalPoints.first : GoalPoints.second;
	task.target_pos = playerPos;
	task.orientate = (goalPoint - playerPos).angle();
	task.needCb = true;
	return task;
}
