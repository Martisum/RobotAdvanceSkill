#include "Skill.h"
#include  "PlaneGeometry.h"
#include "utils/maths.h"
using namespace PlaneGeometry;
using namespace FieldPoint;
using namespace Maths;
extern "C"_declspec(dllexport) PlayerTask player_plan(const WorldModel* model, int robot_id);
//�����������������Ҫ�����ж��Ƿ���Ҫִ�з���
inline bool InsideOurPenaltyArea(const point2f& iPoint) {
	/*
		һ���ܶ��ĵ��жϣ��߼����ʽ�ķ���ֵ���ǲ������ͣ����Ǽ�д
		��||����Ϊ���������˵˵ÿ�������˼
		���������������һ����˵�����ڽ����ڲ������Զ�����ִ�����ţ�����true��
		һ����λ�ڽ����м�ľ����ڲ����������û�ȡ��������ʹ��constant�ڲ��ĳ������бȽ�ʵ�ֵ�
		������λ�ڽ������ߵ������ڲ�����������point2f�ڲ���length���������ʵ�ֵ�
	*/
	return iPoint.x < PENALTY_AREA_R - FIELD_LENGTH_H && abs(iPoint.y) < PENALTY_AREA_L / 2 || (iPoint - Penalty_Arc_Center_Right).length() <
	PENALTY_AREA_R || (iPoint - Penalty_Arc_Center_Left).length() < PENALTY_AREA_R;
}
const double Offset = MAX_ROBOT_SIZE;  //��ƫ��������Ϊ�����˰뾶
//�����������ĺ�����ֵ�������Offset����ô�����������ĵ�����꣬���￼���˻����˵Ĵ�С
const point2f OffsetGoalCenterPoint(-FIELD_LENGTH_H + Offset, 0);
const point2f OffsetGoalLeftPoint(-FIELD_LENGTH_H + Offset, -GOAL_WIDTH_H-3);
const point2f OffsetGoalRightPoint(-FIELD_LENGTH_H + Offset, GOAL_WIDTH_H+3);
PlayerTask player_plan(const WorldModel* model, int robot_id) {
	PlayerTask task;
	const point2f playerPos = model->get_our_player_pos(robot_id);
	const point2f ballPos = model->get_ball_pos();
	const point2f ballVel = model->get_ball_vel();
	const bool* oppExistId = model->get_opp_exist_id();
	//���뵱ǰ�������
	const bool isInsideOurPenaltyArea = InsideOurPenaltyArea(ballPos);
	task.flag = 1;
	task.target_pos = OffsetGoalCenterPoint;
	task.orientate = (ballPos - playerPos).angle();
	task.needCb = isInsideOurPenaltyArea;

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
