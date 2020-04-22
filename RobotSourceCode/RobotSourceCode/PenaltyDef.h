#ifndef PENALTY_H
#define PENALTY_H
#include "util/PlayerTask.h"
class PenaltyDef
{
public:
	PenaltyDef();
	~PenaltyDef();
	PlayerTask plan();
private:
	int opp_penalty_player();
	point2f def_pos(const point2f& p,float dir);
};

typedef Singleton<PenaltyDef> penaltyDef;
#endif