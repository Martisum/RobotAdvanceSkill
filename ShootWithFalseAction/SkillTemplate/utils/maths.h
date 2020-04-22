#ifndef MATHSS_H
#define MATHSS_H
#include <math.h>
#include "constants.h"
#include "vector.h"
#include <vector>
enum RoleStyle{
	KickerS,
	ReceiveS,
	MiddleS,
	TierS,
	AssisterS,
	GoalieS,
	Nothing
};
namespace FieldPoint{
	extern	point2f Goal_Center_Point;
	extern	point2f Penalty_Kick_Point;
	extern	point2f Goal_Left_Point;
	extern  point2f Goal_Right_Point;
	extern  point2f Goal_Center_Left_One_Point;
	extern  point2f Goal_Center_Left_Two_Point;
	extern  point2f Goal_Center_Right_One_Point;
	extern  point2f Goal_Center_Right_Two_Point;
	extern  point2f Goal_Penalty_Area_L_Right;
	extern  point2f Goal_Penalty_Area_L_Left;
	extern  point2f Penalty_Area_L_A;
	extern  point2f Penalty_Area_L_B;
	extern  point2f Penalty_Arc_Center_Right;
	extern  point2f Penalty_Arc_Center_Left;
	extern  point2f Penalty_Rectangle_Left;
	extern  point2f Penalty_Rectangle_Right;
}

namespace Maths{
#define SMALL_NUM   0.00000001 // ±ÜÃâ³ý·¨Òç³ö

#define EPSINON    0.00001

#define dot(u,v)   ((u).x * (v).x + (u).y * (v).y)

#define perp(u,v)  ((u).x * (v).y - (u).y * (v).x)  

#ifndef PI
#define PI						3.1415926535898
#endif

#ifndef RAD2DEG					
#define RAD2DEG					57.295779513082
#endif

#ifndef DEG2RAD					
#define DEG2RAD					0.017453292519943
#endif
	
	
	template<class T>
	T clip(T val, T min_val, T max_val){
		if (max_val < min_val) return val;
		if (val > max_val) val = max_val;
		if (val < min_val) val = min_val;
		return val;
	}

	float normalize(float theta);

    point2f vector2polar(float length, float dir);
	point2f circle_segment_intersection(const point2f& start_point, const double circle_r, const  point2f& end_point);

	point2f archimedes_spiral(const point2f& spiral_center, float spiral_center_size, const point2f& pos, float spiral_buff);
	
	int sign(float d);

	float vector_angle(const point2f& a,const point2f& b,const point2f& c);

	float least_squares(const std::vector<point2f>& point);

	point2f line_perp_across(const point2f& p1, float slope, const point2f& p2);

	bool  in_range(const point2f& p1, const point2f& left_down, const point2f& right_up);

	point2f across_point(const point2f& p1, const point2f& p2, const point2f& q1, const point2f& q2);

	bool is_inside_penatly(const point2f& p);
	
}
#endif