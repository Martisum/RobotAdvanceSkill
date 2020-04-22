#include "RefDef.h"
enum BallArea
{
	Left,
	Middle,
	Right
};
RefDef::RefDef()
{
}

RefDef::~RefDef()
{
}

PlayerTask RefDef::plan(int id, string role){
	//防守需要的参数
	const point2f& goal_center = FieldPoint::Goal_Center_Point;
	const point2f& arc_center_right = FieldPoint::Penalty_Arc_Center_Right;
	const point2f& arc_center_left = FieldPoint::Penalty_Arc_Center_Left;
	const point2f& ball = WorldModel::getInstance()->get_ball_pos();
	const point2f& ref_player = WorldModel::getInstance()->get_our_player_pos(id);
	const point2f& opp_goal = -goal_center;
	const float ref_dir = WorldModel::getInstance()->get_our_player_dir(id);
	const float ball_to_goal_dir = (goal_center - ball).angle();
	const Vehicle* opp_tem = WorldModel::getInstance()->get_opp_team();
	const bool* exist_id = WorldModel::getInstance()->get_opp_exist_id();
	float dist = 0;
	int def_receive_ball =-1;
	//在for循环中找出拿球的对方球员车号给def_receive_ball
	for (int i = 0; i < MAX_ROBOTS; i++){
		if (exist_id[i]){
			const point2f& point = WorldModel::getInstance()->get_opp_player_pos(i);
			if ((point - opp_goal).length() < PENALTY_AREA_R)
				continue;
			else
			{
				float player_ball_dist = (ball - point).length();
				if (player_ball_dist > dist){
					dist = player_ball_dist;
					def_receive_ball = i;
				}		
			}
		}
	}
	if (def_receive_ball == -1) cout << " warning no opp robots receive ball" << endl;
	const point2f& opp_receive_player = WorldModel::getInstance()->get_opp_player_pos(def_receive_ball);
	//得到对方拿球小车到我球门角度
	float opp_receive_goal = (goal_center - opp_receive_player).angle();
	BallArea area;
	if (ball.y + 2  > arc_center_right.y)
		area = Right;
	else if (ball.y + 2 < arc_center_left.y)
		area = Left;
	else
		area = Middle;
	bool have_enough_dist = false;
	float dist_threshold = MAX_ROBOT_SIZE + PENALTY_AREA_R;
	switch (area)
	{
	case Left:
		if ((ball - arc_center_left).length()- RuleParam::Stop_Dist > dist_threshold)
			have_enough_dist = true;
		break;
	case Middle:
		if ((ball - goal_center).length() - RuleParam::Stop_Dist > dist_threshold)
			have_enough_dist = true;
		break;
	case Right:
		if ((ball - arc_center_right).length() - RuleParam::Stop_Dist > dist_threshold)
			have_enough_dist = true;
		break;
	default:
		break;
	}
	PlayerTask task;
	//前锋Kicker的防守，卡住对方拿球小车射门方向
	if (role == "Kicker"){
		task.target_pos = opp_receive_player + Maths::vector2polar(RuleParam::Stop_Dist*2, opp_receive_goal);
		task.orientate = anglemod(opp_receive_goal + PI);
	}
	//中场Receiver的防守，卡住小球射门方向
	else if (role == "Receiver")
	{
		if (have_enough_dist){
			task.target_pos = ball + Maths::vector2polar(RuleParam::Stop_Dist, ball_to_goal_dir);
			task.orientate = anglemod(ball_to_goal_dir + PI);
		}
		else
		{
			float goal_to_ball_dir = (ball - goal_center).angle();
			task.target_pos = goal_center + Maths::vector2polar(95,goal_to_ball_dir);
			task.orientate = goal_to_ball_dir;
		}
	}
	else
	{
		task.target_pos = ref_player;
		task.orientate = ref_dir;
		cout << "not have" << role << "algorithm!!!!!!!!!" << endl;
	}
	return task;
}