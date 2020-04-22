#ifndef MATCHSTATE_H
#define MATCHSTATE_H
enum FieldState
{
	ATTACK,
	TAKENBALL,
	DEFEND,
	NOTFINDBALL
};

class MatchState
{
public:
	MatchState();
	~MatchState();
	void judgeMatchState();
	void ballControlStatus(int bestPlayerID);
	bool simulation(int number);
	void gameState(bool isBallFound);
	FieldState getMatchState();
private:
	FieldState matchState;
	float minBall2OurPlayer;
	float minBall2TheirPlayer;
	bool isBallOurControl;
	bool isBallTheirControl;
};


#endif