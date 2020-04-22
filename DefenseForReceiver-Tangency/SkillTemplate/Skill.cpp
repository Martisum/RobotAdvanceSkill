#include "PlaneGeometry.h"
#include "Skill.h"
#include "utils/maths.h"
extern "C"_declspec(dllexport) PlayerTask player_plan(const WorldModel* model, int robot_id);
using namespace PlaneGeometry;
using namespace FieldPoint;
using namespace Maths;
inline bool InsideOurPenaltyArea(const point2f& iPoint) {
	return iPoint.x < PENALTY_AREA_R - FIELD_LENGTH_H && abs(iPoint.y) < PENALTY_AREA_L / 2 || (iPoint - Penalty_Arc_Center_Right).length() <
	PENALTY_AREA_R || (iPoint - Penalty_Arc_Center_Left).length() < PENALTY_AREA_R;
}
PlayerTask player_plan(const WorldModel* model, int robot_id) {
	PlayerTask task;
	const point2f ballPos = model->get_ball_pos();
	const point2f ballVel = model->get_ball_vel();
	const point2f playerPos = model->get_our_player_pos(robot_id);
	const double rightAngle = (Goal_Right_Point - ballPos).angle();
	const double leftAngle = (Goal_Left_Point - ballPos).angle();
	const double theta = abs(rightAngle - leftAngle);
	const double distance = bound<double, double>(MAX_ROBOT_SIZE / sin(theta / 2),
	                                              (Goal_Center_Point - ballPos).length() - PENALTY_AREA_R,
	                                              2 * MAX_ROBOT_SIZE + BALL_SIZE);
	task.target_pos = ballPos + vector2polar(distance, (rightAngle + leftAngle) / 2);
	task.orientate = (ballPos - playerPos).angle();
	return task;
}
