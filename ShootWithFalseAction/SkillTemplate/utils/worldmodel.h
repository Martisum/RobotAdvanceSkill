#ifndef WORLDMODEL_H
#define WORLDMODEL_H
#include <iostream>
#include "singleton.h"
#include "basevision.h"
#include "historylogger.h"
#include "robot.h"
#include "matchstate.h"
#include "ball.h"
#include "game_state.h"
class WorldModel
{
public:
	WorldModel();
	~WorldModel();
	const std::string& get_referee_msg() const;
	void set_referee_msg(const std::string& ref_msg);
	void set_cycle(int cycle){ current_cycle = cycle; }
	void setMatchState(FieldState state);
	FieldState getMatchState();
	void set_our_team(Vehicle* team){ our = team; }
	const Vehicle* get_our_team()const{ return our; }
	void set_opp_team(Vehicle* team){ opp = team; }
	const Vehicle* get_opp_team()const{ return opp; }
	void set_our_exist_id(bool * id){ our_robots_id = id; }
	const bool* get_our_exist_id()const{ return our_robots_id; }
	void set_opp_exist_id(bool * id){ opp_robots_id = id; }
	const bool* get_opp_exist_id()const{ return opp_robots_id; }
	void set_our_v(int id, point2f v);
	void set_our_rot(int id, float rot);
	void set_our_cmd(int id, const point2f& v, const float& rot);
	const point2f& get_our_cmd_v(int id)const;
	const float& get_our_cmd_rot(int id)const;
	const PlayerVision& get_our_player(int id)const;
	const point2f& get_our_player_pos(int id)const;
	const point2f& get_opp_player_pos(int id)const;
	float get_opp_player_dir(int id)const;
	const point2f& get_our_player_v(int id)const;
	const point2f& get_our_player_last_v(int id)const;
	float get_our_player_dir(int id)const;
	const float get_our_player_last_dir(int id)const;
	const PlayerVision& get_opp_player(int id)const;
	int get_our_goalie()const{ return our_goalie; }
	int get_opp_goalie()const{ return opp_goalie; }
	void set_our_goalie(int goalie_id){ our_goalie = goalie_id; }
	void set_opp_goalie(int goalie_id){ opp_goalie = goalie_id; }
	void set_ball(Ball* b){ match_ball = b; }
	const BallVision& get_ball()const{ return match_ball->get_ball_vision(); }
	const point2f& get_ball_pos(int c)const{ return match_ball->get_pos(c); }
	const point2f& get_ball_pos()const{ return match_ball->get_pos(); }
	const point2f& get_ball_vel()const{ return match_ball->get_vel(); }
	const int  robots_size()const { return max_robots; }
	void set_kick(bool* kick_f) { kick = kick_f; }
	void set_sim_kick(bool* kick){ sim_kick = kick; }
	const bool* get_sim_kick()const{ return sim_kick; }
	bool is_kick(int id)const;
	bool is_sim_kick(int id)const;
	void set_game_state(GameState* state);
	const GameState* game_states()const;
	void set_simulation(bool sim){ is_simulation = sim; }
	bool get_simulation()const{ return is_simulation; }
private:
	static const int max_robots = 12;
	Vehicle* our;
	Vehicle* opp;
	bool* kick;
	bool* sim_kick;
	Ball* match_ball;
	bool* our_robots_id;
	bool* opp_robots_id;
	int our_goalie;
	int opp_goalie;
	int current_cycle;
	std::string referee_msg;
	FieldState match_state;
	GameState* game_state;
	bool is_simulation;
};
#endif