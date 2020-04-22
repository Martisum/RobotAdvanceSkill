#include "PlaneGeometry.h"
#include "Skill.h"
#include "utils/maths.h"
extern "C"_declspec(dllexport) PlayerTask player_plan(const WorldModel* model, int robot_id);
using namespace PlaneGeometry;
using namespace FieldPoint;
using namespace Maths;
PlayerTask player_plan(const WorldModel* model, int robot_id) {
	PlayerTask task;
	const point2f ballPos = model->get_ball_pos();
	const point2f ballVel = model->get_ball_vel();
	const point2f playerPos = model->get_our_player_pos(robot_id);
	task.target_pos = Goal_Center_Point + vector2polar(PENALTY_AREA_R + PENALTY_AREA_L / 2 + MAX_ROBOT_SIZE, (ballPos - Goal_Center_Point).angle());
	task.orientate = (ballPos - playerPos).angle();
	return task;
}
