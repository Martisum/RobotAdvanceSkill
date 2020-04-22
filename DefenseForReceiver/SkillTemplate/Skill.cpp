#include "Skill.h"
#include "utils/maths.h"
#include  "PlaneGeometry.h"
using namespace PlaneGeometry;
using namespace FieldPoint;
using namespace Maths;
extern "C"_declspec(dllexport) PlayerTask player_plan(const WorldModel* model, int robot_id);
PlayerTask player_plan(const WorldModel* model, int robot_id) {
	PlayerTask task;
	const point2f thisRobotPos = model->get_our_player_pos(robot_id);
	const point2f ballPos = model->get_ball_pos();
	task.target_pos = Goal_Left_Point + vector2polar(100, (ballPos - Goal_Center_Point).angle());
	task.orientate = (ballPos - thisRobotPos).angle();
	return task;
}
