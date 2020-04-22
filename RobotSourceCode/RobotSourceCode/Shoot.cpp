#include "shoot.h"
#include "GetBall.h"
#include "Halt.h"
#include "util/maths.h"
#include <math.h>
#include "util/WorldModel.h"
#include <iostream>
#define CENTER_TO_MOUTH 8
#define VISION_ERROR 2
#define FAST_SHOOT 1
namespace {
	bool IsSimulation = false;
	float Param = 1;
}
//Shoot类构造函数，初始化ball_moving_to_head值为false
Shoot::Shoot() { _BallMovingToHead = false; }
Shoot::~Shoot() = default;
PlayerTask Shoot::DoChaseBall(const int iRunnerId) {
	const GetBall getBall;
	return getBall.plan(iRunnerId, iRunnerId);
}

//先停车如果小球在车头吸球嘴上，就射门
PlayerTask Shoot::DoWaitTouch(const int iRunnerId) {
	const point2f& pos = WorldModel::getInstance()->get_our_player_pos(iRunnerId);
	const point2f& ball = WorldModel::getInstance()->get_ball_pos();
	const float& player_dir = WorldModel::getInstance()->get_our_player_dir(iRunnerId);
	//创建一个HaltRobot对象halt，halt.plan方法返回一个PlayerTask任务对象task
	HaltRobot halt;
	PlayerTask task = halt.Plan(iRunnerId);
	//设置task中的吸球开关为true，小车吸球
	task.needCb = true;
	//判断球是否在小车车头吸球嘴上，二个判断条件：小球到车的距离是否小于某个值；车到球的矢量角度是否和车头的矢量角度之差的绝对值是否小于某个值
	if((ball - pos).length() < BALL_SIZE / 2 + MAX_ROBOT_SIZE + 5 && anglemod(player_dir - (ball - pos).angle()) < PI / 6) {
		//设置task中的踢球参数：kickPower踢球力量，needKick平踢球开关，isChipKick挑球开关
		task.kickPower = 127;
		task.needKick = true;
		task.isChipKick = false;
	}
	return task;
}

//
PlayerTask Shoot::DoStopBallAndShoot(int iRunnerId) const {
	PlayerTask task;
	const point2f& pos = WorldModel::getInstance()->get_our_player_pos(iRunnerId);
	const point2f& ball = WorldModel::getInstance()->get_ball_pos();
	const float& dir = WorldModel::getInstance()->get_our_player_dir(iRunnerId);
	const float& dir_ball = fabs(anglemod(dir - (ball - pos).angle()));
	//判断球是否在车头吸球嘴上
	bool orienta_ball = fabs(anglemod(dir - (ball - pos).angle())) < PI / 6;
	//如果球不在车头吸球嘴上，车不动，车头朝向球
	if(!orienta_ball) {
		task.target_pos = pos;
		task.orientate = (ball - pos).angle();
	}
		//如果球在车头吸球嘴上，needCb吸球开关吸球
	else {
		HaltRobot halt;
		task = halt.Plan(iRunnerId);
		task.needCb = true;
	}
	return task;
}

//拿球
PlayerTask Shoot::DoTurnAndShoot(int iRunnerId) {
	PlayerTask task;
	const point2f& pos = WorldModel::getInstance()->get_our_player_pos(iRunnerId);
	const point2f& ball = WorldModel::getInstance()->get_ball_pos();
	const point2f& opp = -FieldPoint::Goal_Center_Point;
	if((ball - pos).length() < 15) {
		task.target_pos = ball + Maths::vector2polar(15, (pos - ball).angle());
		task.orientate = (ball - pos).angle();
	}
	else {
		GetBall get_ball;
		task = get_ball.plan(iRunnerId, iRunnerId);
	}
	/*else
	{
		task.target_pos = ball + Maths::vector2polar(11,(ball - opp).angle());
		task.orientate = (opp - ball).angle();
	}*/
	return task;
}

//调整车位置方向，到对方球门和球的反方向位置
PlayerTask Shoot::DoAdjustDir(int iRunnerId) {
	PlayerTask task;
	const point2f& ball = WorldModel::getInstance()->get_ball_pos();
	const point2f& opp_goal = -FieldPoint::Goal_Center_Point;
	const point2f& player = WorldModel::getInstance()->get_our_player_pos(iRunnerId);
	const point2f& back2ball_p = ball + Maths::vector2polar(20, (ball - opp_goal).angle());
	task.target_pos = back2ball_p;
	task.orientate = (opp_goal - ball).angle();
	return task;
}

//射门
PlayerTask Shoot::DoShoot(int iRunnerId) const {
	const point2f& opp_goal = -FieldPoint::Goal_Center_Point;
	const point2f& ball = WorldModel::getInstance()->get_ball_pos();
	const point2f& player = WorldModel::getInstance()->get_our_player_pos(iRunnerId);
	const float& dir = WorldModel::getInstance()->get_our_player_dir(iRunnerId);
	PlayerTask task;
	bool get_ball = (ball - player).length() < get_ball_threshold - 2.5 && fabs(anglemod(dir - (ball - player).angle())) < PI / 6;
	if(get_ball) {
		task.kickPower = 127;
		task.needKick = true;
		task.isChipKick = false;
	}
	float opp_goal_to_ball = (ball - opp_goal).angle();
	task.target_pos = ball + Maths::vector2polar(FAST_SHOOT, opp_goal_to_ball);
	task.orientate = (opp_goal - ball).angle();
	task.flag = 1;
	return task;
}
PlayerTask Shoot::Plan(int id) {
	cout << "int shoot skill" << endl;
	PlayerTask task;
	//射门需要用到的参数
	const point2f& kick_pos = WorldModel::getInstance()->get_our_player_pos(id);
	const point2f& ball = WorldModel::getInstance()->get_ball_pos();
	const point2f& last_ball = WorldModel::getInstance()->get_ball_pos(1);
	const float kicker_dir = WorldModel::getInstance()->get_our_player_dir(id);
	const point2f& opp = -FieldPoint::Goal_Center_Point;
	float kick2opp_kickdir_angle = anglemod((opp - kick_pos).angle() - kicker_dir);
	//球与射门车位置矢量方向角与射门车车头方向角之差
	float kick2ball_kickdir_angle = anglemod((ball - kick_pos).angle() - kicker_dir);
	bool toward_oppgoal = fabs(kick2opp_kickdir_angle) < PI / 4;
	//球与射门车位置矢量方向角与射门车车头方向角之差小于某个值是为true，即判断球是否在车头方向
	bool ball_front_head = fabs(kick2ball_kickdir_angle) < PI / 3;
	//球当前帧位置和上一帧位置差，即球位移量
	point2f vector_s = ball - last_ball;
	//车头右侧位置
	point2f head_right = kick_pos + Maths::vector2polar(CENTER_TO_MOUTH, anglemod(kicker_dir + PI / 6));
	//车头左侧位置
	point2f head_left = kick_pos + Maths::vector2polar(CENTER_TO_MOUTH, anglemod(kicker_dir - PI / 6));
	//车头中间位置
	point2f head_middle = kick_pos + Maths::vector2polar(7, kicker_dir);
	//车头右侧位置到球位移矢量
	point2f vector_a = head_right - ball;
	//车头左侧位置到球位移矢量
	point2f vector_b = head_left - ball;
	//车头中间位置到球位移矢量
	point2f vector_c = head_middle - ball;
	bool wait_touch, stop_ball;
	bool wait_touch_condition_a, wait_touch_condition_b;
	if(vector_s.length() < VISION_ERROR) { _BallMovingToHead = false; }
	else {
		//求vector_c和vector_s之间的夹角
		float angle_sc = acos(dot(vector_c, vector_s) / (vector_c.length() * vector_s.length()));
		//判断球是否朝车头移动
		_BallMovingToHead = angle_sc < PI / 6 && angle_sc > -PI / 6;
	}
	//射门条件a:球是否在射门车车头方向，并且车头朝向对方球门
	wait_touch_condition_a = ball_front_head && toward_oppgoal;
	//射门条件b:满足条件a的同时是否满足球在车头方向并朝车头运动
	wait_touch_condition_b = _BallMovingToHead && wait_touch_condition_a;
	//停球判断布尔变量
	stop_ball = ball_front_head && _BallMovingToHead && !toward_oppgoal;
	//等球判断布尔变量
	wait_touch = wait_touch_condition_b;
	//ShootMethods枚举变量
	ShootMethods method;
	if(wait_touch)//等球判断，WaitTouch方法
		method = WaitTouch;
	else if(stop_ball)//停球判断, StopBall方法
		method = StopBall;
	else if(!toward_oppgoal)//没有朝向对方球门判断，AdjustDir方法
		method = AdjustDir;
	else if((ball - kick_pos).length() < get_ball_threshold + 5 && anglemod(kicker_dir - (ball - kick_pos).angle()) < PI / 6
	)//判断球在车头吸球嘴位置,ShootBall方法
		method = ShootBall;
	else method = ChaseBall;//拿球方法
	switch(method) {
		case None : break;
		case ChaseBall : task = DoChaseBall(id);
			break;
		case WaitTouch : task = DoWaitTouch(id);
			break;
		case StopBall : task = DoStopBallAndShoot(id);
			break;
		case AdjustDir : task = DoAdjustDir(id);
			break;
		case ShootBall : task = DoShoot(id);
			break;
		default : break;
	}
	cout << "out shoot skill" << endl;
	return task;
}
