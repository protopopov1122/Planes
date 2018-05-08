#ifndef FOOTBALL_GAME_H_
#define FOOTBALL_GAME_H_

#include "football/base.h"
#include "football/GameObject.h"
#include "football/Team.h"
#include "football/Ball.h"
#include "football/Referee.h"
#include <vector>
#include <memory>

namespace Football {

	class Game : public TeamCollection, public KickRegister {
	  public:
	    Game();
		
			GameObject &addGameObject(std::unique_ptr<GameObject>);
			void setKickHandler(KickHandler &);
			
			void visit(GameVisitor &, bool = true) override;
			void visit(GameVisitor &, Position &, float, bool = true) override;
			bool collision(Position &, ControllableObject &, wxSize &) override;
			Ball *getBall() override;
			
			void setBall(std::unique_ptr<Ball>);
			std::pair<Team *, Team *> getTeams() override;
			void setTeams(std::unique_ptr<Team>, std::unique_ptr<Team>);
			void addReferee(std::unique_ptr<Referee>);
			std::pair<uint16_t, uint16_t> getScore();
			void round();
			
			std::vector<std::pair<size_t, size_t>> &getGoals();
			std::pair<Team *, Player *> getLastKick();
			void registerKick(Team &, Player &);
			bool checkGoals();
			bool process(float);
			void setSpeed(float);
	  private:
			bool checkGoal(Rect &, Position &);

			KickHandler *kickHandler;
			float speed;
			std::unique_ptr<Ball> ball;
			std::pair<std::unique_ptr<Team>, std::unique_ptr<Team>> teams;
			std::vector<std::unique_ptr<Referee>> referees;
			std::pair<Team *, Player *> lastKick;
			std::vector<std::pair<size_t, size_t>> goals;
	};
}

#endif