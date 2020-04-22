#ifndef PASSBALL_H
#define PASSBALL_H
#include "util/PlayerTask.h"
class PassBall
{
public:
	PassBall();
	~PassBall();
	PlayerTask plan(int runner_id, int reveiver_id);
	bool is_ready_pass(const point2f& ball, const point2f& passer,const point2f& receiver);
private:
	
};
typedef Singleton<PassBall> passBall;
#endif