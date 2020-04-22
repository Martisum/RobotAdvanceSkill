#include "Skill.h"
#include "../../Intermediary/Intermediary/main.h"

#ifdef _DEBUG
#pragma comment (lib,"worldmodel_lib/Debug/worldmodel_lib.lib")
#pragma comment (lib,"../../Intermediary/Debug/Intermediary.lib")
#else
#pragma comment (lib,"worldmodel_lib/Release/worldmodel_lib.lib")
#pragma comment (lib,"../../Intermediary/Release/Intermediary.lib")
#endif
extern "C" _declspec(dllexport) PlayerTask player_plan(const WorldModel * model, int robot_id);


PlayerTask player_plan(const WorldModel* model, int robot_id)
{
	PlayerTask task;
	task.target_pos = GetControlState() ? GetTargetPos() : point2f(0, 0);
	task.orientate = (model->get_ball_pos() - model->get_our_player_pos(robot_id)).angle();
	return task;
}