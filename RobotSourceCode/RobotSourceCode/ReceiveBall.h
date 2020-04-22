#ifndef RECEIVEBALL_H
#define RECEIVEBALL_H
#include "util/PlayerTask.h"
class ReceiveBall
{
public:
	ReceiveBall();
	~ReceiveBall();
	PlayerTask plan(int runner_id);
private:

};

typedef Singleton<ReceiveBall> receiveBall;
#endif