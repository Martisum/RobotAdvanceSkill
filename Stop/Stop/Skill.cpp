#include "utils/worldmodel.h"
#include "utils/constants.h"
#include "utils/PlayerTask.h"
#ifdef _DEBUG
#pragma comment (lib,"worldmodel_lib/Debug/worldmodel_lib.lib")
#else
#pragma comment (lib,"worldmodel_lib/Release/worldmodel_lib.lib")
#endif
extern "C" _declspec(dllexport) PlayerTask player_plan(const WorldModel * model, int robot_id);
PlayerTask player_plan(const WorldModel* model, int robot_id) {
	PlayerTask task;
	task.target_pos = model->get_our_player_pos(robot_id);
	task.orientate = model->get_our_player_dir(robot_id);
	return task;
}
