#include "football/Team.h"
#include <cmath>
#include <iostream>

namespace Football {

	HalfdefenderController::HalfdefenderController(ControllableObject &obj, TeamCollection &game, Team &team, float speed, float kickStrength, float range)
		: PlayerController::PlayerController(obj, game, team, speed, kickStrength), range(range) {}
	
	void HalfdefenderController::play(PassiveObject &ball, float millis) {
		float energy = this->team.getParameters().getEnergy();
		Position &dest = this->team.getKickDestination();
		float delta = fabs(dest.x - ball.getPosition().x);
		if (delta > this->range) {
			const float HIT_DISTANCE = 0.0075f;
			if (this->distanceTo(ball.getPosition()) > HIT_DISTANCE) {
				if (this->team.getNearestPlayer(ball.getPosition()) == &this->object) {
					this->chase(millis, ball.getPosition(), energy);
				} else {
					this->smartChase(millis, ball.getPosition(), energy);
				}
			} else {
				this->smartKick(ball, dest, this->kickStrength);
			}
		} else {
			this->random(millis, 1.0f - dest.x, fabs(dest.x - 0.5f), energy);
		}
	}
	
	HalfdefenderControllerFactory::HalfdefenderControllerFactory(TeamCollection &game, Team &team, float speed, float kickStrength, float range)
		: PlayerControllerFactory::PlayerControllerFactory(game, team, speed, kickStrength), range(range) {}
	
	std::unique_ptr<ObjectController> HalfdefenderControllerFactory::create(ControllableObject &obj, KickHandler *kick) {
		std::unique_ptr<HalfdefenderController> ctrl = std::make_unique<HalfdefenderController>(obj, this->gameObjects, this->team, this->speed, this->kickStrength, this->range);
		ctrl->setKickHandler(kick);
		return ctrl;
	}
}