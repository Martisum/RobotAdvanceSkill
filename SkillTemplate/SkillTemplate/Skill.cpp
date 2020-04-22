#include "utils/PlayerTask.h"
#include "utils/constants.h"
#include "utils/worldmodel.h"
#ifdef _DEBUG
#pragma comment (lib,"worldmodel_lib\\Debug\\worldmodel_lib.lib")
#else
#pragma comment (lib,"worldmodel_lib\\Release\\worldmodel_lib.lib")
#endif
extern "C" _declspec(dllexport) PlayerTask player_plan(const WorldModel * model, int robot_id);
PlayerTask player_plan(const WorldModel* model, int robot_id)
{
	PlayerTask task;
	return task;
}