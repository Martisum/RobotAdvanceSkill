#include "PlaneGeometry.h"
#include "Skill.h"
#include "utils/maths.h"
extern "C"_declspec(dllexport) PlayerTask player_plan(const WorldModel* model, int robot_id);
using namespace PlaneGeometry;
using namespace FieldPoint;
inline bool InsideOurPenaltyArea(const point2f& iPoint) {
	return iPoint.x < PENALTY_AREA_R - FIELD_LENGTH_H && abs(iPoint.y) < PENALTY_AREA_L / 2 || (iPoint - Penalty_Arc_Center_Right).length() <
	PENALTY_AREA_R || (iPoint - Penalty_Arc_Center_Left).length() < PENALTY_AREA_R;
}
PlayerTask player_plan(const WorldModel* model, int robot_id) {
	PlayerTask task;
	const point2f ballPos = model->get_ball_pos();
	const point2f ballVel = model->get_ball_vel();
	if(ballVel.length() < 5) { }
	else { }
	return task;
}
