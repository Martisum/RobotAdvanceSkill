#include "PenaltyDef.h"
#include "ParamReader.h"

namespace{
	float goalie_penalty_def_buf = 20;
}
/*
PenaltyDef::PenaltyDef(){
	//srand((int)time(NULL));

	DECLARE_PARAM_READER_BEGIN(PlayBotSkillParam)
	READ_PARAM(goalie_penalty_def_buf)
	DECLARE_PARAM_READER_END

}

PenaltyDef::~PenaltyDef()
{
}
*/

//获得离我球门最近的对方球员车号
int PenaltyDef::opp_penalty_player(){
	int opp_penalty_id = -1;
	const bool* exist_id = WorldModel::getInstance()->get_opp_exist_id();
	const point2f& our_goal = FieldPoint::Goal_Center_Point;
	float dist = 9999;
	for (int i = 0; i < MAX_ROBOTS; i++){
		if (exist_id[i]){
			const point2f& pos = WorldModel::getInstance()->get_opp_player_pos(i);
			float goal_opp_dist = (pos - our_goal).length();
			if (goal_opp_dist<dist){
				dist = goal_opp_dist;
				opp_penalty_id = i;
			};
		}
	}
	return opp_penalty_id;
}
 //计算防守位置
point2f PenaltyDef::def_pos(const point2f& p, float dir){
	float x = -FIELD_LENGTH_H + goalie_penalty_def_buf;
	float y = p.y + tanf(dir)*(x - p.x);
	int convert = y > 0 ? 1 : -1;
	if (y > 30 || y < -30)
		y = 30 * convert;
	return point2f(x,y);
}	

PlayerTask PenaltyDef::plan(){
	PlayerTask task;
	int opp_kicker = opp_penalty_player();
	const point2f& ball = WorldModel::getInstance()->get_ball_pos();
	const point2f& our_goal = FieldPoint::Goal_Center_Point;
	if (opp_kicker == -1) {
		task.target_pos = our_goal + point2f(10,0);
		task.orientate = (task.target_pos - our_goal).angle();
		cout << "no opp penalty kicker" << endl;
		return task;
	}
	const point2f& penalty_kicker = WorldModel::getInstance()->get_opp_player_pos(opp_kicker);
	float opp_dir = WorldModel::getInstance()->get_opp_player_dir(opp_kicker);
	task.target_pos = def_pos(penalty_kicker,opp_dir);
	task.orientate = (ball - task.target_pos).angle();
	return task;
}