#include "Skill.h"
#include "utils/maths.h"
#include  "PlaneGeometry.h"
using namespace PlaneGeometry;
using namespace FieldPoint;
using namespace Maths;
//extern "C"_declspec(dllexport) PlayerTask player_plan(const WorldModel* model, int robot_id);
inline bool InsideOurPenaltyArea(const point2f& iPoint) {
	return iPoint.x < PENALTY_AREA_R - FIELD_LENGTH_H && abs(iPoint.y) < PENALTY_AREA_L / 2 || (iPoint - Penalty_Arc_Center_Right).length() <
	PENALTY_AREA_R || (iPoint - Penalty_Arc_Center_Left).length() < PENALTY_AREA_R;
}
PlayerTask player_plan(const WorldModel* model, int robot_id) {
	PlayerTask task;
	const point2f ballPos = model->get_ball_pos();
	const point2f ballVel = model->get_ball_vel();
	const point2f playerPos = model->get_our_player_pos(robot_id);
	const bool* oppExistId = model->get_opp_exist_id();
	task.flag = 1;
	task.target_pos = Goal_Center_Point;
	task.orientate = (ballPos - playerPos).angle();
	task.needCb = InsideOurPenaltyArea(ballPos);
	for(int i = 0; i < MAX_ROBOTS; i++) {
		if(oppExistId[i]) {
			const point2f oppPlayerPos = model->get_opp_player_pos(i);
			const double oppPlayerDir = model->get_opp_player_dir(i);
			if((ballPos - oppPlayerPos).length() < 15 && abs(anglemod(oppPlayerDir - (ballPos - oppPlayerPos).angle())) < PI / 4) {
				const point2f goalPoint = CrossPoint(oppPlayerPos, ballPos, Goal_Left_Point, Goal_Right_Point);
				if (abs(goalPoint.Y()) < GOAL_WIDTH_H) task.target_pos = goalPoint;
				return task;
			}
		}
	}
	if(ballVel.X() < 0 && ballVel.length() > 5) {
		const point2f goalPoint = CrossPoint(ballPos, ballPos + ballVel, Goal_Left_Point, Goal_Right_Point);
		if(abs(goalPoint.Y()) < GOAL_WIDTH_H) task.target_pos = goalPoint;
	}
	else if(task.needCb) task.target_pos = ballPos;
	return task;
}
