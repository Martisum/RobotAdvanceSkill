/*
	�������ǻ����˳�����ץ��Ĵ���
	���޸���һ���жϣ���Ϊ���Ƿϻ�
	��ĵط���û�и�
*/
#include "Skill.h"
#include  "utils/maths.h"
extern "C"_declspec(dllexport) PlayerTask player_plan(const WorldModel* model, int robot_id);
PlayerTask player_plan(const WorldModel* model, int robot_id) {
	PlayerTask task;
	//��ȡ��ǰ�����˱�ŵ�����
	const point2f playerPos = model->get_our_player_pos(robot_id);
	//��ȡ��ǰ�������
	const point2f ballPos = model->get_ball_pos();
	//��ȡ��ǰ�����˵ĳ�����һ���Ƕ�ֵ
	const double playerDir = model->get_our_player_dir(robot_id);
	//��ȡ��ͻ���������λ�õļнǣ�����Ϊ�˱�֤�нǴ����㣬��Ҫȡ����ֵ
	double theta1 = abs(playerDir - (ballPos - playerPos).angle());
	//��֤�����С�ڦ�
	theta1 = theta1 < PI ? theta1 : 2 * PI - theta1;
	//������仰ע�͵Ŀ�Ҫ�ɲ�Ҫ����Ҫ�ǿ��Եģ���������û����
	//�����������ͻ����˵ĽǶ�С����ʮ��Ȼ��߾��������İ뾶���ϻ�����ͷ���ľ��룬�ǾͰ�Ŀ�����Ϊ���λ��
//	if(theta1 < PI / 4 || (ballPos - playerPos).length() > ROBOT_HEAD + BALL_SIZE) task.target_pos = ballPos;
	task.target_pos = ballPos;
	//ת��ָ��ѻ����˳������λ�ã���������
	task.orientate = (ballPos - playerPos).angle();
	/*
	ע�⣬���ڻ����˵�����ע���λ�ڻ���������
	Ȼ���������ע���Ҳ��λ��������ģ�������������ʼ�ղ����غ�
	��˻����˻��޴�������ǰ������
	*/
	return task;
}
