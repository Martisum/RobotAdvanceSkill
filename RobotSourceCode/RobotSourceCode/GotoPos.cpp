#include "GotoPos.h"
GotoPos::GotoPos()
{
}

GotoPos::~GotoPos()
{
}

PlayerTask GotoPos::plan(float x, float y, float dir){
	PlayerTask task;
	task.target_pos.x = x;
	task.target_pos.y = y;
	task.orientate = dir;
	return task;
}