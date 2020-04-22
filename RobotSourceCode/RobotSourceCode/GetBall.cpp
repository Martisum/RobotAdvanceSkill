#include "GetBall.h"
#include "util/maths.h"
#include "util/constants.h"
#include "util/WorldModel.h"
double spiral_buff = 8.0;
double get_ball_buf = -4;
double do_spiral_dist = 30;
double do_spiral_buff = 0;
double around_ball_dist = 30;
double vision_error = 3;
bool isSimulation = false;
int do_spiral_max_cnt = 70;
#define frame_rate  60.0
float away_ball_dist_x = 20;//这个值越大 拿球越平滑
GetBall::GetBall() {
	isSimulation = WorldModel::getInstance()->get_simulation();
	isSimulation ? get_ball_buf = -4 : get_ball_buf = 5;
	isSimulation ? away_ball_dist_x = 20 : away_ball_dist_x = 40;
}
GetBall::~GetBall() {}

//判断朝向对方球门角度范围，如果大于-90度小于90度，返回true
bool GetBall::toward_opp_goal(float dir) { return dir < PI / 2 && dir > -PI / 2; }
float GetBall::ball_x_angle() {
	vector<point2f> ball_points;
	ball_points.resize(8);
	for(int i = 0; i < 8; i++) {
		const point2f& temp_points = WorldModel::getInstance()->get_ball_pos(i);
		ball_points.push_back(temp_points);
	}
	return Maths::least_squares(ball_points);
}
//robot_id为拿球小车车号，receiver_id为接球小车车号
PlayerTask GetBall::plan(int robot_id, int receiver_id) const {
	//创建PlayerTask对象
	PlayerTask task;
	//以下为执行拿球需要的参数，相关常量查看constaants.h
	//获得小球当前图像帧坐标位置，重点：小球的坐标信息都以图像帧为最小单位从视觉机接收并存储，可以把球坐标看成是一个个数组，数组索引是图像帧号，数组元素是坐标信息
	const point2f& ball = WorldModel::getInstance()->get_ball_pos();
	//获得小球当前帧的上一帧图像坐标信息
	const point2f& last_ball = WorldModel::getInstance()->get_ball_pos(1);
	//获得我方receiver_id小车坐标位置信息
	const point2f& receive_ball_player = WorldModel::getInstance()->get_our_player_pos(receiver_id);
	//获得我方robot_id小车坐标信息
	const point2f& get_ball_player = WorldModel::getInstance()->get_our_player_pos(robot_id);
	//敌方球门中点
	const point2f& opp_goal = -FieldPoint::Goal_Center_Point;
	//我方receier_id小车朝向信息，注意：小车朝向为车头垂直方向与场地x轴正方向逆时针夹角
	const float rece_dir = WorldModel::getInstance()->get_our_player_dir(receiver_id);
	//获得以receive_ball_player为原点的极坐标，ROBOY_HEAD为极坐标length,rece_dir为极坐标angle
	const point2f& rece_head_pos = receive_ball_player + Maths::vector2polar(ROBOT_HEAD, rece_dir);
	//获得我方robot_id小车朝向信息
	const float dir = WorldModel::getInstance()->get_our_player_dir(robot_id);
	//获得receive_ball_player到ball向量的角度，注意：所有角度计算为向量与场地x轴正方向逆时针夹角
	float receive2ball = (ball - receive_ball_player).angle();
	//获得对方球门到球的向量角度
	float opp_goal2ball = (ball - opp_goal).angle();
	//获得ball到对方球门的向量角度
	float ball2opp_goal = (opp_goal - ball).angle();
	//获得对方球门到球的向量长度
	float ball_away_goal = (ball - opp_goal).length();
	//获得球到get_ball_player的向量长度
	float player_away_ball = (get_ball_player - ball).length();
	//获得对方球门到get_ball_player的长度
	float player_away_goal = (get_ball_player - opp_goal).length();
	//获得球当前坐标到上一坐标位置向量的长度
	float ball_moving_dist = (ball - last_ball).length();
	//判断dir与对方球门的角度关系，看toward_opp_goal函数
	bool is_toward_opp_goal = toward_opp_goal(dir);
	//判断小车是否在球与对方球门之间
	bool ball_behind_player = ball_away_goal + BALL_SIZE + MAX_ROBOT_SIZE > player_away_goal;
	//判断小球是否运动
	bool ball_moving = ball_moving_dist < 0.8 ? false : true;
	//判断get_ball_player小车到ball向量角绝对值是否小于75度
	bool player_toward_ball = fabs((ball - get_ball_player).angle() - dir) < PI / 2 - PI / 12 ? true : false;
	bool direct_get_ball = !ball_moving;
	bool across_ball;
	bool ball_move2target;
	float ball_moving_dir = (ball - last_ball).angle();
	point2f ball_with_vel = ball + Maths::vector2polar(ball_moving_dist, ball_moving_dir);
	if(!ball_moving)
		//小球位移为当前位置
		ball_with_vel = ball;
	float ball_to_player = (get_ball_player - ball_with_vel).angle();
	//球车方向和小车方向的夹角，其中球车方向为小球与小车中心点的矢量方向、小车方向为垂直车头方向
	float ball_player_dir_angle = (ball - get_ball_player).angle() - dir;
	//判断小球是否在吸球嘴附近
	bool ball_beside_player_mouth = (ball - get_ball_player).length() < 14 && fabs(ball_player_dir_angle) > PI / 4 && fabs(
		ball_player_dir_angle) < PI / 2;
	if(receiver_id == robot_id) {
		//判断x轴方向get_ball_Player小车与球的位置关系，小车在球上侧，返回true	
		bool ball_x_boundary_right = ball.x - 2 < get_ball_player.x ? true : false;
		//判断y轴方向get_ball_player小车与球的位置关系，小车在球左侧，返回true
		bool ball_y_boundary_right = ball.y - 2 < get_ball_player.y ? true : false;
		//判断小球与get_ball_player车的位置关系执行拿球
		if(!ball_x_boundary_right) {
			//给robot_id小车设置任务中的目标点坐标，就是让小车跑到某个点，该点以ball_with_vel为极坐标原点
			task.target_pos = ball_with_vel + Maths::vector2polar(BALL_SIZE / 2 + MAX_ROBOT_SIZE + get_ball_buf, opp_goal2ball);
		}
		else {
			if(ball_y_boundary_right)
				//给robot_id小车设置任务中的目标点坐标，直接设置x,y
				task.target_pos.set(ball_with_vel.x - away_ball_dist_x, ball_with_vel.y + 35);
			else task.target_pos.set(ball_with_vel.x - away_ball_dist_x, ball_with_vel.y - 35);
		}
		task.orientate = (opp_goal - ball).angle();
	}
	else {   //判断球与get_ball_palyer、receive_ball_player之间的位置关系，如果x轴方向球在两车下侧，得到true
		bool all_on_ball_x_boundary_left = ball.x - 2 < get_ball_player.x && ball.x - 2 < receive_ball_player.x;
		//如果x轴方向球在两车上侧，得到true
		bool all_on_ball_x_boundary_right = ball.x - 2 > get_ball_player.x && ball.x - 2 > receive_ball_player.x;
		//判断y轴方向get_ball_player小车与球的位置关系，小车在球左侧，得到true
		bool executer_onball_y_boundary_right = ball.y - 2 < get_ball_player.y ? true : false;
		//判断小球与拿球车、接球车之间的位置关系执行拿球
		if(all_on_ball_x_boundary_right) {
			if(executer_onball_y_boundary_right)
				//设置task任务目标点坐标
				task.target_pos.set(ball_with_vel.x + away_ball_dist_x, ball_with_vel.y + 35);
			else task.target_pos.set(ball_with_vel.x + away_ball_dist_x, ball_with_vel.y - 35);
		}
		else if(all_on_ball_x_boundary_left) {
			if(executer_onball_y_boundary_right) task.target_pos.set(ball_with_vel.x - away_ball_dist_x, ball_with_vel.y + 35);
			else task.target_pos.set(ball_with_vel.x - away_ball_dist_x, ball_with_vel.y - 35);
		}
		else { task.target_pos = ball_with_vel + Maths::vector2polar(BALL_SIZE / 2 + MAX_ROBOT_SIZE + get_ball_buf, receive2ball); }
		task.orientate = (rece_head_pos - ball).angle();
	}
	//判断小球在拿球车吸球嘴附近执行拿球
	if(ball_beside_player_mouth) {
		//
		task.target_pos = ball + Maths::vector2polar(20, anglemod(dir + PI));
	}
	return task;
}
