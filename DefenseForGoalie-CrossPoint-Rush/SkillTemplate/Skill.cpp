#include "Skill.h"
#include "utils/maths.h"
#include  "PlaneGeometry.h"
using namespace PlaneGeometry;
using namespace FieldPoint;
using namespace Maths;
extern "C"_declspec(dllexport) PlayerTask player_plan(const WorldModel* model, int robot_id);
inline bool InsideOurPenaltyArea(const point2f& iPoint) {
	return iPoint.x < PENALTY_AREA_R - FIELD_LENGTH_H && abs(iPoint.y) < PENALTY_AREA_L / 2 || (iPoint - Penalty_Arc_Center_Right).length() <
	PENALTY_AREA_R || (iPoint - Penalty_Arc_Center_Left).length() < PENALTY_AREA_R;
}
PlayerTask player_plan(const WorldModel* model, int robot_id) {
	PlayerTask task;
	const point2f playerPos = model->get_our_player_pos(robot_id);
	const point2f ballPos = model->get_ball_pos();
	const point2f ballVel = model->get_ball_vel();
	task.flag = 1;
	task.target_pos = Goal_Center_Point;
	task.orientate = (ballPos - playerPos).angle();
	if(ballVel.X() < 0 && ballVel.length() > 5) {
		const point2f goalPoint = CrossPoint(ballPos, ballPos + ballVel, Goal_Left_Point, Goal_Right_Point);
		if(abs(goalPoint.Y()) < GOAL_WIDTH_H)
			task.target_pos = (playerPos - goalPoint).length() < 1 ? goalPoint : playerPos + (goalPoint - playerPos) * 5;
	}
	else if(InsideOurPenaltyArea(ballPos)) task.target_pos = ballPos;
	return task;
}
