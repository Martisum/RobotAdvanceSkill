#ifndef BASETASK_H
#define BASETASK_H
#include "vector.h"
#include "singleton.h"
#include <string>
#define error_robot_id 9999
#define get_ball_threshold 16
enum RobotRole
{
	NoRole,
	Kicker,
	LeftMiddle,
	RightMiddle,
	LeftBack,
	RightBack,
	GoalieRole
};
class PlayerRole
{
public:
	PlayerRole(){ role = NoRole; num_id = error_robot_id; };
	~PlayerRole(){};
	RobotRole role;
	int num_id;
};


//抽象基本任务；依据skill需要可以设置不同的任务；skill执行的任务继承PlayeTask；
class PlayerTask
{
public:
	PlayerTask(){ memset(this,0,sizeof(PlayerTask)); };
	PlayerTask(const PlayerTask& task){
		memcpy(this,&task,sizeof(PlayerTask));
	}
	~PlayerTask(){};
	int flag;
	RobotRole role;                                             //球员角色
	point2f target_pos;											// 全局目标点
	double orientate;												// 全局目标到点朝向
	point2f global_vel;											// 全局目标到点平动速度	
	double rot_vel;													// 全局目标到点转动速度
	int rot_dir;														// 旋转的方向
	/// 运动参数 ： 用于底层运动控制 ，指定标签辅助
	double maxAcceleration;										// 最大加速度
	double maxDeceleration;										// 最大减速度

	/// 踢球参数 ： 用于平射挑射控制 ，默认使用
	bool needKick;													// 踢球动作执行开关
	bool isPass;													// 是否进行传球
	bool needCb;
	bool isChipKick;												// 挑球还是平射
	double kickPrecision;											// 踢球朝向精度
	double kickPower;												// 踢球力度
	double chipKickPower;											// 挑球力度	
};

class CTaskMeditator
{
public:
	CTaskMeditator(){ 
		task_list = new PlayerTask[MaxRobotsTask]();
		is_at_point = new bool[MaxRobotsTask]();
	}
	~CTaskMeditator(){
		if (task_list) {
			delete task_list;
			task_list = NULL;
		}
		if (is_at_point){
			delete is_at_point;
			is_at_point = NULL;
		}
	};
	void set_task(int id,const PlayerTask& task){
		if (id < 0 || id>11) return;
		task_list[id] = task;
	}

	const PlayerTask& get_task(int id){
		if (id < 0 || id>11) return task_list[0];
		return task_list[id];
	}
	const bool arrive_point(int id, const point2f& current_pos, const float& dir){
		if (id < 0 || id>11) return false;
		if ((current_pos - task_list[id].target_pos).length() < arrive_point_range/*&&(fabs(anglemod(dir - task_list[id].orientate)))<5*PI/180*/)
			is_at_point[id] = true;
		else
			is_at_point[id] = false;
		return is_at_point[id];
	}
private:
	static const int MaxRobotsTask = 6;
	static const int arrive_point_range = 10;
	PlayerTask *task_list;
	bool* is_at_point;
};

typedef Singleton<CTaskMeditator> TaskMeditator;
#endif