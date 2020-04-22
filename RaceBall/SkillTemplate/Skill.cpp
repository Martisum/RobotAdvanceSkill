#include "Skill.h"
#include "PlaneGeometry.h"
extern "C"_declspec(dllexport) PlayerTask player_plan(const WorldModel* model, int robot_id);
using namespace PlaneGeometry;
PlayerTask player_plan(const WorldModel* model, int robot_id) {
	PlayerTask task;
	const point2f playerPos = model->get_our_player_pos(robot_id);
	const point2f ballPos = model->get_ball_pos();
	const point2f ballVel = model->get_ball_vel();
	task.flag = 1;
	task.target_pos = ballVel.length() > 10 ? CrossPoint(ballPos, ballPos + ballVel, playerPos, playerPos + point2f(-ballVel.y, ballVel.x)) : ballPos;
	task.orientate = (ballPos - playerPos).angle();
	task.needCb = true;
	return task;
}
