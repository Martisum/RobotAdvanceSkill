#ifndef GETBALL_H
#define GETBALL_H
#include "util/PlayerTask.h"
//#include "./vision/basevision.h"
class GetBall {
public:
	GetBall();
	~GetBall();
	PlayerTask plan(int runner_id, int receiver_id) const;
	static bool toward_opp_goal(float dir);
	static float ball_x_angle();
private:
};
typedef Singleton<GetBall> getBall;
#endif
