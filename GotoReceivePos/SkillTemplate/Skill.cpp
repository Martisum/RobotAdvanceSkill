#include "PlaneGeometry.h"
#include "Skill.h"
#include "utils/constants.h"
#include "utils/maths.h"
extern "C"_declspec(dllexport) PlayerTask player_plan(const WorldModel* model, int robot_id);
using namespace PlaneGeometry;
PlayerTask player_plan(const WorldModel* model, int robot_id) {
	PlayerTask task;
	//��ȡִ�н������λ��Ҫ�Ĳ��������ֲ���ע��ɲο�GetBall.cpp
	const point2f& opp = -FieldPoint::Goal_Center_Point;
	const point2f& ball = model->get_ball_pos();
	//convert�Ƿ�ת��������ball���Ұ볡ʱ��convertΪ-1����ball����볡��convertΪ1
	int convert = ball.y > 0 || fabs(ball.y) < 3 ? -1 : 1;
	bool is_front = ball.x > -3;
	//�жϵз����Ƿ��赲ִ�н������λ
	//�ж����Ƿ���ǰ��ִ�н������λ
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
