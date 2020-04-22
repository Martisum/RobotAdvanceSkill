#include "NormalDef.h"
#include "util/maths.h"
enum PenaltyArea
{
	RightArc,
	MiddleRectangle,
	LeftArc
};
NormalDef::NormalDef()
{
}

NormalDef::~NormalDef()
{
}

PlayerTask NormalDef::plan(int robot_id){
	//创建PlayerTask对象task，task对象是一个任务方法集合
	PlayerTask task;
	//以下为执行防守需要的参数，部分参数解释可以参看GetBall.cpp
	const point2f& goal = FieldPoint::Goal_Center_Point;
	const point2f& arc_center_right = FieldPoint::Penalty_Arc_Center_Right;
	const point2f& arc_center_left = FieldPoint::Penalty_Arc_Center_Left;
	const point2f& rectangle_left = FieldPoint::Penalty_Rectangle_Left;
	const point2f& rectangle_right = FieldPoint::Penalty_Rectangle_Right;
	const point2f& ball = WorldModel::getInstance()->get_ball_pos();
	//area为枚举变量，根据不同的ball位置，设置不同的枚举值
	PenaltyArea area;
	if (ball.y > arc_center_right.y)
		area = RightArc;
	else if (ball.y < arc_center_left.y)
		area = LeftArc;
	else
		area = MiddleRectangle;
	//switch根据不同的area值，设置不同的task
	switch (area)
	{
	case RightArc:
		//任务小车的朝向角及目标点
		task.orientate = (ball - goal).angle();
		task.target_pos = goal + Maths::vector2polar(PENALTY_AREA_R + MAX_ROBOT_SIZE+PENALTY_AREA_R/2, task.orientate);
		break;
	case MiddleRectangle:
		task.orientate = (ball - goal).angle();
		//across_point(p1, p2, p3, p4)函数是求p1p2线段和p3p4线段的交点
		task.target_pos = Maths::across_point(rectangle_left,rectangle_right,ball,goal);
		break;
	case LeftArc:
		task.orientate = (ball - goal).angle();
		task.target_pos = goal + Maths::vector2polar(PENALTY_AREA_R + MAX_ROBOT_SIZE + PENALTY_AREA_R / 2, task.orientate);
		break;
	default:
		break;
	}

	return task;
}