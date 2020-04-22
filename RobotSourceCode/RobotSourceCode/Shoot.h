#ifndef SHOOT_H
#define SHOOT_H
#include "util/PlayerTask.h"
enum ShootMethods {
	None,
	ShootBall,
	StopBall,
	ChaseBall,
	WaitTouch,
	AdjustDir,
	GoAndTurnKick,
};
class Shoot {
public:
	Shoot();
	~Shoot();
	PlayerTask Plan(int iRunnerId);
private:
	PlayerTask DoShoot(int iRunnerId) const;
	static PlayerTask DoChaseBall(int iRunnerId);
	static PlayerTask DoWaitTouch(int iRunnerId);
	PlayerTask DoStopBallAndShoot(int iRunnerId) const;
	static PlayerTask DoTurnAndShoot(int iRunnerId);
	static PlayerTask DoAdjustDir(int iRunnerId);
	bool _BallMovingToHead;
};
typedef Singleton<Shoot> shoot;
#endif
