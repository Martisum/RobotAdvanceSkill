#include "PlaneGeometry.h"
#include "Skill.h"
extern "C"_declspec(dllexport) PlayerTask player_plan(const WorldModel* model, int robot_id);
using namespace PlaneGeometry;
PlayerTask player_plan(const WorldModel* model, int robot_id) {
	PlayerTask task;
	const point2f ballPos = model->get_ball_pos();
	const double playerDir = model->get_our_player_dir(robot_id);
	task.target_pos = ballPos;
	task.chipKickPower = 127;
	task.kickPower = 127;
	task.needCb = true;
	task.isChipKick = true;
	task.needKick = true;
	task.orientate = playerDir;
	return task;
}
