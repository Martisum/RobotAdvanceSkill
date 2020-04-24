#include "Skill.h"
#include  "PlaneGeometry.h"
#include "utils/maths.h"
using namespace PlaneGeometry;
using namespace FieldPoint;
using namespace Maths;
extern "C"_declspec(dllexport) PlayerTask player_plan(const WorldModel* model, int robot_id);
//下面这个内联函数主要用于判断是否需要执行防守
inline bool InsideOurPenaltyArea(const point2f& iPoint) {
	/*
		一个很恶心的判断，逻辑表达式的返回值就是布朗类型，这是简写
		以||符号为间隔，我来说说每个或的意思
		下列条件任意成立一个就说明球在禁区内部，所以都可以执行守门（返回true）
		一、球位于禁区中间的矩形内部。这是利用获取球的坐标和使用constant内部的常量进行比较实现的
		二、球位于禁区两边的扇形内部。这是利用point2f内部的length函数求距离实现的
	*/
	return iPoint.x < PENALTY_AREA_R - FIELD_LENGTH_H && abs(iPoint.y) < PENALTY_AREA_L / 2 || (iPoint - Penalty_Arc_Center_Right).length() <
	PENALTY_AREA_R || (iPoint - Penalty_Arc_Center_Left).length() < PENALTY_AREA_R;
}
const double Offset = MAX_ROBOT_SIZE;  //把偏移量设置为机器人半径
//下面这个坐标的横坐标值如果不加Offset，那么就是球门中心点的坐标，这里考虑了机器人的大小
const point2f OffsetGoalCenterPoint(-FIELD_LENGTH_H + Offset, 0);
const point2f OffsetGoalLeftPoint(-FIELD_LENGTH_H + Offset, -GOAL_WIDTH_H-3);
const point2f OffsetGoalRightPoint(-FIELD_LENGTH_H + Offset, GOAL_WIDTH_H+3);
PlayerTask player_plan(const WorldModel* model, int robot_id) {
	PlayerTask task;
	const point2f playerPos = model->get_our_player_pos(robot_id);
	const point2f ballPos = model->get_ball_pos();
	const point2f ballVel = model->get_ball_vel();
	const bool* oppExistId = model->get_opp_exist_id();
	//传入当前球的坐标
	const bool isInsideOurPenaltyArea = InsideOurPenaltyArea(ballPos);
	task.flag = 1;
	task.target_pos = OffsetGoalCenterPoint;
	task.orientate = (ballPos - playerPos).angle();
	task.needCb = isInsideOurPenaltyArea;
	
	//下面这个if判断是执行场外防守
	//当球还未进入禁区，而且速度较小，执行“位于直线上的防守”
	if(ballPos.x > FIELD_LENGTH_H - 230 && abs(ballPos.y) > FIELD_WIDTH_H - 100 && ballVel.length() < 5)
		for(int i = 0; i < MAX_ROBOTS; i++)
			if(oppExistId[i]) {
				const point2f oppPlayerPos = model->get_opp_player_pos(i);
				const double oppPlayerDir = model->get_opp_player_dir(i);
				if((ballPos - oppPlayerPos).length() < 15 && abs(anglemod(oppPlayerDir - (ballPos - oppPlayerPos).angle())) < PI / 4) {
					const point2f goalPoint = CrossPoint(oppPlayerPos, ballPos, Goal_Left_Point, Goal_Right_Point);
					if(abs(goalPoint.Y()) < GOAL_WIDTH_H) {
						task.target_pos = CrossPoint(oppPlayerPos, ballPos, OffsetGoalLeftPoint, OffsetGoalRightPoint);
						return task;
					}
				}
			}

	if(ballVel.length() > 5 && ballVel.X() < 0) {
		const point2f goalPoint = CrossPoint(ballPos, ballPos + ballVel, Goal_Left_Point, Goal_Right_Point);
		if(abs(goalPoint.Y()) < GOAL_WIDTH_H) task.target_pos = CrossPoint(ballPos, ballPos + ballVel, OffsetGoalLeftPoint, OffsetGoalRightPoint);
	}
	else if(isInsideOurPenaltyArea) task.target_pos = ballPos;
	return task;
}
