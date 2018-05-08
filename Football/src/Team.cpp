#include "football/Team.h"
#include <cfloat>
#include <iostream>
#include <cmath>

namespace Football {
	
	TeamLayout::TeamLayout() : goalkeeper(true) {}
	
	bool TeamLayout::isGoalkeeperEnabled() {
		return this->goalkeeper;
	}
	
	std::vector<TeamPlayer> &TeamLayout::getActivePlayers() {
		return this->activePlayers;
	}
	
	void TeamLayout::addActivePlayer(ActivePlayerType type, Position pos) {
		this->activePlayers.push_back(TeamPlayer(type, pos));
	}
	
	void TeamLayout::enableGoalkeeper(bool eg) {
		this->goalkeeper = eg;
	}

	TeamParameters::TeamParameters() : score(0), energy(0.5f) {}
	
	uint16_t TeamParameters::getScore() {
		return this->score;
	}
	
	void TeamParameters::incScore() {
		this->score++;
	}
	
	void TeamParameters::resetScore() {
		this->score = 0;
	}
	
	float TeamParameters::getEnergy() {
		return this->energy;
	}
	
	void TeamParameters::setEnergy(float energy) {
		this->energy = energy;
	}
	
	Team::Team(TeamCollection &g, KickRegister &kReg, std::unique_ptr<Drawable> draw, Position dest, Rect goal, size_t id)
		: teamCollection(g), kickRegister(kReg), id(id), playerDrawable(std::move(draw)), destination(dest), goal(goal) {
		this->attackers = std::make_unique<AttackerControllerFactory>(this->teamCollection, *this, 0.1f, 0.2f);
		this->defenders = std::make_unique<DefenderControllerFactory>(this->teamCollection, *this, 0.08f, 0.16f);
		this->halfdefenders = std::make_unique<HalfdefenderControllerFactory>(this->teamCollection, *this, 0.09f, 0.18f, 0.7f);
		Position goalkeeper(1 - this->destination.x, this->destination.y);
		this->goalkeepers = std::make_unique<GoalkeeperControllerFactory>(this->teamCollection, *this, 0.08f, 0.14f, 0.15f);
	}
	
	TeamLayout &Team::getLayout() {
		return this->layout;
	}

	TeamParameters &Team::getParameters() {
		return this->parameters;
	}
	
	size_t Team::getID() {
		return this->id;
	}
	
	void Team::reset() {
		this->players.clear();
	}
	
	void Team::spawn(bool respawn) {
		std::vector<bool> old;
		for (std::size_t i = 0; i < this->players.size(); i++) {
			old.push_back(this->players.at(i)->isEnabled());
		}
		this->reset();
		Position base(this->goal.x + this->goal.w / 2, this->goal.y + this->goal.h / 2);
		PlayerControllerFactory *fact = nullptr;
		for (const TeamPlayer &pl : this->layout.getActivePlayers()) {
			switch (pl.type) {
				case ActivePlayerType::Attacker:
					fact = this->attackers.get();
					break;
				case ActivePlayerType::Defender:
					fact = this->defenders.get();
					break;
				case ActivePlayerType::Halfdefender:
					fact = this->halfdefenders.get();
					break;
			}			
			std::unique_ptr<Player> player = std::make_unique<Player>(*this->playerDrawable, *this, this->kickRegister, *fact, this->players.size() + 1);
			Position pos(fabs(base.x - pl.position.x), pl.position.x);
			player->setPosition(pos);
			if (respawn && this->players.size() < old.size()) {
				player->enable(old.at(this->players.size()));
			}
			this->players.push_back(std::move(player));
		}
		if (this->layout.isGoalkeeperEnabled()) {
			std::unique_ptr<Player> player = std::make_unique<Player>(*this->playerDrawable, *this, this->kickRegister, *this->goalkeepers, this->players.size() + 1);
			player->setPosition(base);
			if (respawn && this->players.size() < old.size()) {
				player->enable(old.at(this->players.size()));
			}
			this->players.push_back(std::move(player));
		}
	}
	
	Rect &Team::getGoal() {
		return this->goal;
	}

	void Team::visit(GameVisitor &v, bool only_enabled) {
		for (std::size_t i = 0; i < this->players.size(); i++) {
			if (this->players.at(i)->isEnabled() || !only_enabled) {
				v.visit(*this->players.at(i));
			}
		}
	}
	
	void Team::visit(GameVisitor &v, Position &center, float radius) {
		for (std::size_t i = 0; i < this->players.size(); i++) {
			Player &obj = *this->players.at(i);
			float dx = center.x - obj.getPosition().x;
			float dy = center.y - obj.getPosition().y;
			if (hypot(dx, dy) <= radius && obj.isEnabled()) {
				v.visit(obj);
			}
		}	
	}
	
	bool Team::playerCollides(ControllableObject &obj, Position &pos, wxSize &size) {
		for (std::size_t i = 0; i < this->players.size(); i++) {
			if (this->players.at(i)->isEnabled() && GameObject::collision(obj, *this->players.at(i), pos, size)) {
				return true;
			}
		}
		return false;
	}
	
	GameObject *Team::getNearestPlayer(Position &position) {
		GameObject *nearest = nullptr;
		float minimal = FLT_MAX;
		for (std::size_t i = 0; i < this->players.size(); i++) {
			Player &pl = *this->players.at(i);
			Position &plpos = pl.getPosition();
			float dist = hypot(position.x - plpos.x, position.y - plpos.y);
			if (dist < minimal) {
				nearest = &pl;
				minimal = dist;
			}
		}
		return nearest;
	}
	
	Position &Team::getKickDestination() {
		return this->destination;
	}
	
	Player &Team::getRandomPlayer() {
		return *this->players.at(rand() % this->players.size());
	}
}