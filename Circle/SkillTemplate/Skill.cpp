#include "PlaneGeometry.h"
#include "Skill.h"
#include "utils/maths.h"
extern "C"_declspec(dllexport) PlayerTask player_plan(const WorldModel* model, int robot_id);
using namespace PlaneGeometry;
PlayerTask player_plan(const WorldModel* model, int robot_id) {
	PlayerTask task;
	const point2f player_pos = model->get_our_player_pos(robot_id);
	const point2f goalie_pos = model->get_our_player_pos(model->get_our_goalie());
	const bool* our_exist_id = model->get_our_exist_id();
	const bool* opp_exist_id = model->get_opp_exist_id();
	int c1 = 0, c2 = 0;
	const double dt = PI / 20;
	bool find = false;
	int id{};
	for(int i = 0, c = 0; i <= MAX_ROBOTS; i++) {
		if(our_exist_id[i]) {
			if(i == robot_id) break;
			c1++;
		}
	}
	for(int i = 0; i <= MAX_ROBOTS; i++) {
		if(opp_exist_id[i]) {
			if(c2 == c1) {
				id = i;
				break;
			}
			c2++;
		}
		if(find) break;
	}
	task.target_pos = - model->get_opp_player_pos(id);
	task.orientate = (task.target_pos - player_pos).angle();
	return task;
}
