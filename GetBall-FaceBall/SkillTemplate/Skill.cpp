/*
	本代码是机器人朝向球抓球的代码
	我修改了一句判断，因为那是废话
	别的地方都没有改
*/
#include "Skill.h"
#include  "utils/maths.h"
extern "C"_declspec(dllexport) PlayerTask player_plan(const WorldModel* model, int robot_id);
PlayerTask player_plan(const WorldModel* model, int robot_id) {
	PlayerTask task;
	//获取当前机器人编号的坐标
	const point2f playerPos = model->get_our_player_pos(robot_id);
	//获取当前球的坐标
	const point2f ballPos = model->get_ball_pos();
	//获取当前机器人的朝向，是一个角度值
	const double playerDir = model->get_our_player_dir(robot_id);
	//获取球和机器人坐标位置的夹角，而且为了保证夹角大于零，需要取绝对值
	double theta1 = abs(playerDir - (ballPos - playerPos).angle());
	//保证这个角小于π
	theta1 = theta1 < PI ? theta1 : 2 * PI - theta1;
	//下面这句话注释的可要可不要，不要是可以的，反正测试没问题
	//大意是如果球和机器人的角度小于四十五度或者距离大于球的半径加上机器人头部的距离，那就把目标点设为球的位置
//	if(theta1 < PI / 4 || (ballPos - playerPos).length() > ROBOT_HEAD + BALL_SIZE) task.target_pos = ballPos;
	task.target_pos = ballPos;
	//转向指令，把机器人朝向球的位置，方便拿球
	task.orientate = (ballPos - playerPos).angle();
	/*
	注意，由于机器人的坐标注册点位于机器人中心
	然后球坐标的注册点也是位于球的中心，所以两个坐标始终不会重合
	因此机器人会愚蠢地推球前进……
	*/
	return task;
}
