#include "PlaneGeometry.h"
#include "Skill.h"
#include "utils/maths.h"
extern "C"_declspec(dllexport) PlayerTask player_plan(const WorldModel* model, int robot_id);
using namespace PlaneGeometry;
using namespace Maths;
PlayerTask player_plan(const WorldModel* model, int robot_id) {
	PlayerTask task;
	const point2f ballPos = model->get_ball_pos();
	const bool* oppExitId = model->get_opp_exist_id();
	const point2f playerPos = model->get_our_player_pos(robot_id);
	int id = -1;
	float min_dist = 99999;
	for(int i = 0; i < MAX_ROBOTS; i++) {
		if(oppExitId[i] && i != model->get_opp_goalie()) {
			if((model->get_opp_player_pos(i) - model->get_ball_pos()).length() < min_dist) {
				id = i;
				min_dist = (model->get_opp_player_pos(i) - model->get_ball_pos()).length();
			}
		}
	}
	const point2f theirPos = model->get_opp_player_pos(id);
	float their2BallDir = (ballPos - theirPos).angle();
	task.target_pos = ballPos + vector2polar(55, their2BallDir);
	task.orientate = (ballPos - playerPos).angle();
	return task;
}
