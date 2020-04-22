#ifndef  VEHICLE_H
#define  VEHICLE_H
#include "basevision.h"
#include "historylogger.h"
#include "FilteredObject.h"
#define FrameRate 60
class Vehicle
{
public:
	Vehicle();
	~Vehicle();
	void filter_mobile_vel(const point2f& pos, bool is_lost,bool is_own);
	void fileter_rotate_vel(const point2f& orientate, bool is_lost, bool is_own);
	void set_cur_cycle(int c){ cur_cycle = c ; } //视觉周期与数组对应
	void set_vel(){ log->getLogger(cur_cycle).set_vel(mobile_vel);};
	const point2f& get_vel() const{ return mobile_vel; }
	const float get_rot(){ return rotate_vel; }
	void set_rotate_v(){ log->getLogger(cur_cycle).set_rot(rotate_vel); };
	void set_robot_properties(const Robot& properties, bool is_exist, bool is_own);
	void predicte_robot(bool is_lost);
	point2f predicte_pos(const PlayerVision& vision);
	const Robot& get_robot() const{ return log->getLogger(cur_cycle).player; }
	RobotLogger* get_robot_log(){ return log; }
public:
	int lost_frame;
private:
	FilteredObject mobile_filter;
	FilteredObject rotate_filter;
	point2f  mobile_vel;
	float    rotate_vel;
	RobotLogger* log;
	int cur_cycle;
};


#endif