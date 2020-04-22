#ifndef BASEVISION_H
#define BASEVISION_H
#include "vector.h"
#include <map>
#include <vector>
#define ERRORNUM 9999
#define CAMERAS 2
#define MaxPlayer 6
/*场地信息选择如下数据结构原因如下：
1：使用vector可以动态的储存场上的视觉数据；提高程序效率；操作视觉数据方便
2： 如果使用数组，只能先静态定义好场上的视觉储存空间例如：蓝队机器人：buleRobot[6]；同时操作数据很不方便（融合处理删去相同车号数据）
3： 如果使用map<int,Vector2dFloat> buleRobot可以很好的操作数据和动态分配，但是无法满足储存场上相同车号的数据，因为key唯一*/
enum Team
{
	BULE = 0,
	YELLOW = 1
};

/*机器人车号对应的位置*/
typedef struct RobotProperty{
	int  id;
	float orientation;
	point2f pos;
	RobotProperty(){ id = ERRORNUM; orientation = 0.0; pos.x = 0; pos.y = 0; }
	void zero(){ id = ERRORNUM; orientation = 0.0; pos.x = 0; pos.y = 0; }
}Robot;

//暴露给用户数据类型；原则是对场上信息不做任何加工
/*FieldMSG信息是场上蓝队/黄队/球的位置信息 vectors使用resize先分配内存 程序会更快*/
typedef struct FieldMSG{
	int cameraId;
	int frame;
	std::vector<point2f>  ball;
	std::vector<Robot> buleRobot;
	std::vector<Robot> yellowRobot;
	FieldMSG(){
		cameraId = ERRORNUM;
		frame = ERRORNUM;
	}
} FieldVisionMsg;

//以下数据类型暴露给决策模块使用
class MobileObject
{
public:
	MobileObject(){ v = point2f(0, 0); }
	~MobileObject(){}
	void set_vel(const point2f t){ v = t; }
	void set_vel(float vx, float vy){ v.set(vx, vy);}
	point2f vel() const { return v; }
protected:
	point2f v;//平移速度
};

class PlayerObject{
public:
	PlayerObject():rotVel(0){}
	PlayerObject(float rot):rotVel(rot){}
	~PlayerObject(){}
	void set_rot(float rot){ rotVel = rot; }
	float rot() const { return rotVel; }
protected:
	float rotVel;// 旋转速度
};

///为方便后面决策模块方便调用视觉数据，替换掉原始数据类型FieldVisionMsg，把球和球员数据独立开来；
//储存在Logger中球数据类型如下：成员变量没有私有化原因：频繁调用
class BallVision:public MobileObject
{
public:
	BallVision(){ isBallPredict = false; isExist = false; pos.x = 0.0; pos.y = 0.0; }
	~BallVision(){}
	bool isBallPredict;//标识是否是预测数据
	bool isExist;
	point2f pos;
};

//单个球员数据类型如下,成员变量没有私有化原因：频繁调用
class PlayerVision:public PlayerObject,public MobileObject
{
public:
	PlayerVision(){ isRobotPredict = false;  isExist = false; cmd_rot = 0.0; cmd_v.x = 0.0; cmd_v.y = 0.0; player.zero(); }
	PlayerVision(const Robot& r, bool isPredict, bool isFound, bool valid) :player(r), isRobotPredict(isPredict), isExist(isFound){}
	~PlayerVision(){}
	bool isRobotPredict;
	bool isExist;
	Robot player;
	point2f cmd_v;
	float cmd_rot;
};

template <class item_t>
class AllocaArray{
public:
	explicit AllocaArray(){
		for (int i = 0; i < MAXSIZE; i++){
			p[i] = nullptr;
		}
		cnt = 0;
	}

	~AllocaArray(){}

	item_t* alloc(int size){
		if (cnt >= MAXSIZE) cnt = 0;
		if (p[cnt]) free();
		p[cnt++] = new item_t[size];
		return p[cnt - 1];
	}

	void free(){
		delete[]p[cnt];
	}
private:
	int cnt;
	const static int MAXSIZE = 16;//注意MAXSIZE要与Logger中maxsize要对应
	item_t* p[MAXSIZE];
};

typedef AllocaArray<PlayerVision> AllocPlayer;

/*该接口易扩展*/
class Vision
{
public:
	Vision();
	~Vision();
	virtual void processVision(FieldVisionMsg* visionData) = 0;
protected:
	
};
#endif