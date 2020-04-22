#include "goalie.h"
#include "GetBall.h"
Goalie::Goalie()
{
}

Goalie::~Goalie()
{
}

//id为守门员车号
PlayerTask Goalie::plan(int id){
	//创建PlayerTask对象
	PlayerTask task;
	//执行守门员防守需要的参数，部分参数可以看GetBAll.cpp中注释，相关常量查看constants.h
	const point2f& golie_pos = WorldModel::getInstance()->get_our_player_pos(id);
	const point2f& ball = WorldModel::getInstance()->get_ball_pos();
	const float dir = WorldModel::getInstance()->get_our_player_dir(id);
	const point2f& goal = FieldPoint::Goal_Center_Point;
	const point2f& opp_goal = -FieldPoint::Goal_Center_Point;
	bool ball_inside_penalty = is_inside_penalty(ball);
	//判断小球在禁区内执行守门员防守
	if (ball_inside_penalty){
		//判断小球在控球嘴上执行挑球
		if ((ball - golie_pos).length() < BALL_SIZE / 2 + MAX_ROBOT_SIZE + 2 && (anglemod(dir - (ball - golie_pos).angle()) < PI / 6)){
			task.kickPower = 127;
			task.needKick = true;
			task.isChipKick = true;
		}
		task.orientate = (ball - golie_pos).angle();
		task.target_pos = ball + Maths::vector2polar(BALL_SIZE / 2 + MAX_ROBOT_SIZE - 2, task.orientate);
	}
	//小球不在禁区内执行守门员防守
	else
	{
		task.orientate = (ball - goal).angle();
		task.target_pos = goal + Maths::vector2polar(30,task.orientate);
	}
	return task;
}
//判断球是否在禁区内
bool Goalie::is_inside_penalty(const point2f& p){
	//a为球门左侧点，b为球门右侧点
	const point2f& a = FieldPoint::Penalty_Arc_Center_Left;
	const point2f& b = FieldPoint::Penalty_Arc_Center_Right;
	if (fabs(p.y) < PENALTY_AREA_L / 2)
		return p.x < -FIELD_LENGTH_H + PENALTY_AREA_R && p.x > -FIELD_LENGTH_H;//PENALTY_AREA_R=80
	else if (p.y < 0)
		return (p - a).length() < PENALTY_AREA_R && p.x > -FIELD_LENGTH_H;
	else
		return (p - b).length() < PENALTY_AREA_R&& p.x > -FIELD_LENGTH_H;
}