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
float away_ball_dist_x = 20;//���ֵԽ�� ����Խƽ��
GetBall::GetBall() {
	isSimulation = WorldModel::getInstance()->get_simulation();
	isSimulation ? get_ball_buf = -4 : get_ball_buf = 5;
	isSimulation ? away_ball_dist_x = 20 : away_ball_dist_x = 40;
}
GetBall::~GetBall() {}

//�жϳ���Է����ŽǶȷ�Χ���������-90��С��90�ȣ�����true
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
//robot_idΪ����С�����ţ�receiver_idΪ����С������
PlayerTask GetBall::plan(int robot_id, int receiver_id) const {
	//����PlayerTask����
	PlayerTask task;
	//����Ϊִ��������Ҫ�Ĳ�������س����鿴constaants.h
	//���С��ǰͼ��֡����λ�ã��ص㣺С���������Ϣ����ͼ��֡Ϊ��С��λ���Ӿ������ղ��洢�����԰������꿴����һ�������飬����������ͼ��֡�ţ�����Ԫ����������Ϣ
	const point2f& ball = WorldModel::getInstance()->get_ball_pos();
	//���С��ǰ֡����һ֡ͼ��������Ϣ
	const point2f& last_ball = WorldModel::getInstance()->get_ball_pos(1);
	//����ҷ�receiver_idС������λ����Ϣ
	const point2f& receive_ball_player = WorldModel::getInstance()->get_our_player_pos(receiver_id);
	//����ҷ�robot_idС��������Ϣ
	const point2f& get_ball_player = WorldModel::getInstance()->get_our_player_pos(robot_id);
	//�з������е�
	const point2f& opp_goal = -FieldPoint::Goal_Center_Point;
	//�ҷ�receier_idС��������Ϣ��ע�⣺С������Ϊ��ͷ��ֱ�����볡��x����������ʱ��н�
	const float rece_dir = WorldModel::getInstance()->get_our_player_dir(receiver_id);
	//�����receive_ball_playerΪԭ��ļ����꣬ROBOY_HEADΪ������length,rece_dirΪ������angle
	const point2f& rece_head_pos = receive_ball_player + Maths::vector2polar(ROBOT_HEAD, rece_dir);
	//����ҷ�robot_idС��������Ϣ
	const float dir = WorldModel::getInstance()->get_our_player_dir(robot_id);
	//���receive_ball_player��ball�����ĽǶȣ�ע�⣺���нǶȼ���Ϊ�����볡��x����������ʱ��н�
	float receive2ball = (ball - receive_ball_player).angle();
	//��öԷ����ŵ���������Ƕ�
	float opp_goal2ball = (ball - opp_goal).angle();
	//���ball���Է����ŵ������Ƕ�
	float ball2opp_goal = (opp_goal - ball).angle();
	//��öԷ����ŵ������������
	float ball_away_goal = (ball - opp_goal).length();
	//�����get_ball_player����������
	float player_away_ball = (get_ball_player - ball).length();
	//��öԷ����ŵ�get_ball_player�ĳ���
	float player_away_goal = (get_ball_player - opp_goal).length();
	//�����ǰ���굽��һ����λ�������ĳ���
	float ball_moving_dist = (ball - last_ball).length();
	//�ж�dir��Է����ŵĽǶȹ�ϵ����toward_opp_goal����
	bool is_toward_opp_goal = toward_opp_goal(dir);
	//�ж�С���Ƿ�������Է�����֮��
	bool ball_behind_player = ball_away_goal + BALL_SIZE + MAX_ROBOT_SIZE > player_away_goal;
	//�ж�С���Ƿ��˶�
	bool ball_moving = ball_moving_dist < 0.8 ? false : true;
	//�ж�get_ball_playerС����ball�����Ǿ���ֵ�Ƿ�С��75��
	bool player_toward_ball = fabs((ball - get_ball_player).angle() - dir) < PI / 2 - PI / 12 ? true : false;
	bool direct_get_ball = !ball_moving;
	bool across_ball;
	bool ball_move2target;
	float ball_moving_dir = (ball - last_ball).angle();
	point2f ball_with_vel = ball + Maths::vector2polar(ball_moving_dist, ball_moving_dir);
	if(!ball_moving)
		//С��λ��Ϊ��ǰλ��
		ball_with_vel = ball;
	float ball_to_player = (get_ball_player - ball_with_vel).angle();
	//�򳵷����С������ļнǣ������򳵷���ΪС����С�����ĵ��ʸ������С������Ϊ��ֱ��ͷ����
	float ball_player_dir_angle = (ball - get_ball_player).angle() - dir;
	//�ж�С���Ƿ��������츽��
	bool ball_beside_player_mouth = (ball - get_ball_player).length() < 14 && fabs(ball_player_dir_angle) > PI / 4 && fabs(
		ball_player_dir_angle) < PI / 2;
	if(receiver_id == robot_id) {
		//�ж�x�᷽��get_ball_PlayerС�������λ�ù�ϵ��С�������ϲ࣬����true	
		bool ball_x_boundary_right = ball.x - 2 < get_ball_player.x ? true : false;
		//�ж�y�᷽��get_ball_playerС�������λ�ù�ϵ��С��������࣬����true
		bool ball_y_boundary_right = ball.y - 2 < get_ball_player.y ? true : false;
		//�ж�С����get_ball_player����λ�ù�ϵִ������
		if(!ball_x_boundary_right) {
			//��robot_idС�����������е�Ŀ������꣬������С���ܵ�ĳ���㣬�õ���ball_with_velΪ������ԭ��
			task.target_pos = ball_with_vel + Maths::vector2polar(BALL_SIZE / 2 + MAX_ROBOT_SIZE + get_ball_buf, opp_goal2ball);
		}
		else {
			if(ball_y_boundary_right)
				//��robot_idС�����������е�Ŀ������ֱ꣬������x,y
				task.target_pos.set(ball_with_vel.x - away_ball_dist_x, ball_with_vel.y + 35);
			else task.target_pos.set(ball_with_vel.x - away_ball_dist_x, ball_with_vel.y - 35);
		}
		task.orientate = (opp_goal - ball).angle();
	}
	else {   //�ж�����get_ball_palyer��receive_ball_player֮���λ�ù�ϵ�����x�᷽�����������²࣬�õ�true
		bool all_on_ball_x_boundary_left = ball.x - 2 < get_ball_player.x && ball.x - 2 < receive_ball_player.x;
		//���x�᷽�����������ϲ࣬�õ�true
		bool all_on_ball_x_boundary_right = ball.x - 2 > get_ball_player.x && ball.x - 2 > receive_ball_player.x;
		//�ж�y�᷽��get_ball_playerС�������λ�ù�ϵ��С��������࣬�õ�true
		bool executer_onball_y_boundary_right = ball.y - 2 < get_ball_player.y ? true : false;
		//�ж�С�������򳵡�����֮���λ�ù�ϵִ������
		if(all_on_ball_x_boundary_right) {
			if(executer_onball_y_boundary_right)
				//����task����Ŀ�������
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
	//�ж�С�������������츽��ִ������
	if(ball_beside_player_mouth) {
		//
		task.target_pos = ball + Maths::vector2polar(20, anglemod(dir + PI));
	}
	return task;
}
