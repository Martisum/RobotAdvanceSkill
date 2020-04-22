#ifndef BALL_H
#define BALL_H
#include "FilteredObject.h"
#include "historylogger.h"
class Ball
{
public:
	Ball();
	~Ball();
	void set_cycle(int cycle){ cur_cycle = cycle; }
	void filter_vel(const point2f observe_pos, bool is_lost);
	point2f predicte_pos(const BallVision last_cycle);
	void set_ball_vision(const point2f pos, bool is_lost);
	void predicte_ball(bool is_lost);
	const BallVision& get_ball_vision() { return log.getLogger(cur_cycle); }
	const point2f& get_pos(int cycle){ return log.getLogger(cur_cycle -cycle).pos; }
	const point2f& get_pos(){ return log.getLogger(cur_cycle).pos; }
	const point2f& get_vel(){ return vel; }
public:
	int lost_frame;
private:
	FilteredObject ball_filter;
	BallLogger log;
	point2f vel;
	int proc_frame;
	int cur_cycle;
};


#endif