#include <fstream>
#include "Skill.h"
extern "C"_declspec(dllexport) PlayerTask player_plan(const WorldModel* model, int robot_id);
PlayerTask player_plan(const WorldModel* model, int robot_id) {
	PlayerTask task;
	const point2f ballPos = model->get_ball_pos();
	const point2f playerPos = model->get_our_player_pos(robot_id);
	ifstream fKickPower("KickPower"), fChipKickPower("ChipKickPower");
	fKickPower >> task.kickPower;
	fChipKickPower >> task.chipKickPower;
	task.target_pos = model->get_ball_pos();
	task.orientate = (ballPos - playerPos).angle();
	task.needCb = true;
	task.needKick = true;
	task.isChipKick = true;
	return task;
}
