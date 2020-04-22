#ifndef HISTORYLOGGER_H
#define HISTORYLOGGER_H
#include "basevision.h"
template<class T>
class HistoryLogger
{
public:
	HistoryLogger(){};
	~HistoryLogger(){};
	T & getLogger(int cycle) { 
		if (cycle < 0) 
			return logger[0];
		else 
			return logger[cycle%maxLogger];
	}

	int maxLoggerSize(){ return maxLogger; }
private:
	static const int maxLogger = 17;
	T  logger[maxLogger];
};
typedef HistoryLogger<BallVision> BallLogger;
typedef HistoryLogger<PlayerVision> RobotLogger;

#endif