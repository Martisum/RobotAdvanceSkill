#include "PlaneGeometry.h"
#include "Skill.h"
#include "utils/constants.h"
#include "utils/maths.h"
extern "C"_declspec(dllexport) PlayerTask player_plan(const WorldModel* model, int robot_id);
using namespace PlaneGeometry;
PlayerTask player_plan(const WorldModel* model, int robot_id) {
	PlayerTask task;
	//获取执行接球点跑位需要的参数，部分参数注解可参考GetBall.cpp
	const point2f& opp = -FieldPoint::Goal_Center_Point;
	const point2f& ball = model->get_ball_pos();
	//convert是反转参数，当ball在右半场时，convert为-1，当ball在左半场，convert为1
	int convert = ball.y > 0 || fabs(ball.y) < 3 ? -1 : 1;
	bool is_front = ball.x > -3;
	//判断敌方车是否阻挡执行接球点跑位
	//判断球是否在前场执行接球点跑位
	if(is_front) {
		task.target_pos.x = ball.x - 50;
		task.target_pos.y = convert * (FIELD_WIDTH_H - 50);
		task.orientate = (opp - task.target_pos).angle();
	}
	else {
		task.target_pos.x = 20;
		task.target_pos.y = convert * (FIELD_WIDTH_H - 50);
		task.orientate = (ball - task.target_pos).angle();
	}
	return task;
}
