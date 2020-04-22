#include "Skill.h"
#include "PlaneGeometry.h"
#include "utils/maths.h"
extern "C"_declspec(dllexport) PlayerTask player_plan(const WorldModel* model, int robot_id);
using namespace PlaneGeometry;
using namespace Maths;
const double GoalDistance = 20;
const double MoveDistance = 25;
PlayerTask player_plan(const WorldModel* model, int robot_id) {
	PlayerTask task;
	const point2f playerPos = model->get_our_player_pos(robot_id);
	const point2f ballPos = model->get_ball_pos();
	const bool* ourExistId = model->get_opp_exist_id();
	int theOtherPlayerId{};
	for(int i = 0; i < MAX_ROBOTS; i++) {
		if(ourExistId[i] && i != robot_id && i != model->get_our_goalie()) {
			theOtherPlayerId = i;
			break;
		}
	}
	const point2f theOtherPlayerPos = model->get_our_player_pos(theOtherPlayerId);
	const point2f goalPoint = ballPos + vector2polar(GoalDistance, (ballPos - theOtherPlayerPos).angle());
	if(abs(anglemod((playerPos - ballPos).angle() - (theOtherPlayerPos - ballPos).angle())) < atan2(MoveDistance, GoalDistance)) {
		point2f tempPos1 = ballPos + vector2polar(GoalDistance, (ballPos - theOtherPlayerPos).angle() + PI / 2);
		point2f tempPos2 = ballPos + vector2polar(GoalDistance, (ballPos - theOtherPlayerPos).angle() - PI / 2);
		task.target_pos = (tempPos1 - playerPos).length() < (tempPos2 - playerPos).length() ? tempPos1 : tempPos2;
	}
	else task.target_pos = goalPoint;
	task.orientate = (theOtherPlayerPos - playerPos).angle();
	return task;
}
