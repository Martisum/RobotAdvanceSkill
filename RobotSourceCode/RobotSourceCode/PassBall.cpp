#include "PassBall.h"
#include "rolematch/MunkresMatch.h"
#include "util/maths.h"
#include "util/constants.h"
//#include "ParamReader.h"
#include <iostream>
using namespace std;
float head_len = 7.0;
#define fast_pass 3
namespace{
	bool isSimulation = false;
	float param = 2;
}
PassBall::PassBall()
{

}

PassBall::~PassBall()
{
	
}
//判断是否可以传球
bool PassBall::is_ready_pass(const point2f& ball ,const point2f& passer, const point2f& receiver){
	//接球车到球矢量角度
	float receiver_to_ball = (ball - receiver).angle();
	//球到传球车矢量角度
	float ball_to_passer= (passer - ball).angle();
	//两个矢量角度之差小于某个值，判断是否可以传球
	bool pass = fabs(receiver_to_ball - ball_to_passer) < 0.5;
	return pass;
}
//runner_id 传球车号 reveiver_id 接球车号
PlayerTask PassBall::plan(int runner_id, int reveiver_id){
	PlayerTask task;
	//获取执行传球需要用到的参数，部分参数解释可以参考GetBall.cpp
	//获取reveiver_id小车的视觉信息
	const PlayerVision& rece_msg = WorldModel::getInstance()->get_our_player(reveiver_id);
	//获取runner_id小车的视觉信息
	const PlayerVision& excute_msg = WorldModel::getInstance()->get_our_player(runner_id);
	float rece_dir = rece_msg.player.orientation;
	float excute_dir = excute_msg.player.orientation;
 	const point2f& rece_pos = rece_msg.player.pos;
	const point2f& excute_pos = excute_msg.player.pos;
	const point2f& ball = WorldModel::getInstance()->get_ball_pos();
	const point2f& opp_goal = -FieldPoint::Goal_Center_Point;
	float ball_to_our_goal = (FieldPoint::Goal_Center_Point - ball).angle();
	float rece_to_ball = (ball - rece_pos).angle();
	point2f receive_head = rece_pos + Maths::vector2polar(head_len, rece_msg.player.orientation);
	float pass_dir = (receive_head - ball).angle();
	//判断球是否在小车控球嘴上，从两个参数着手：1.判断ball到车的距离是否小于某个值，2.车头方向和车到球矢量角度之差值是否小于某个值
	bool get_ball = (ball - excute_pos).length() < get_ball_threshold-1.5  && (fabs(anglemod(excute_dir - (ball - excute_pos).angle())) < PI / 6);
	//如果reveiver_id和runner_id是同一车，则直接射门
	if (reveiver_id == runner_id){
		if (get_ball){
			//执行平击踢球，力度为最大127
			task.kickPower = 127;
			//踢球开关
			task.needKick = true;
			//挑球开关
			task.isChipKick = false;
		}
		//执行拿球，并指向对方球门，对方球门到球矢量的角度
		float opp_goal_to_ball = (ball - opp_goal).angle();
		task.target_pos = ball + Maths::vector2polar(fast_pass, opp_goal_to_ball);
		task.orientate = (opp_goal - ball).angle();
		return task;
	}
	//判断并执行传球
 	if (is_ready_pass(ball,excute_pos,rece_pos) ){
		if (get_ball){
			task.kickPower = 50;
			task.needKick = true;
			task.isChipKick = false;
		}
		task.target_pos = ball + Maths::vector2polar(fast_pass, rece_to_ball);
		//printf("kicke ball\n");
	}
	else
	{
		task.target_pos = ball + Maths::vector2polar(BALL_SIZE / 2 + MAX_ROBOT_SIZE +12, rece_to_ball);
	}
	task.orientate = pass_dir;
	//flag = 1表示小车加速度*2
	task.flag = 1;
	return task;
	
	
}






