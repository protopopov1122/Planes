#include "football/Team.h"
#include <cmath>
#include <iostream>

namespace Football {

	AttackerController::AttackerController(ControllableObject &obj, TeamCollection &game, Team &team, float speed, float kickStrength)
		: PlayerController::PlayerController(obj, game, team, speed, kickStrength) {}
	
	void AttackerController::play(PassiveObject &ball, float millis) {
		const float HIT_DISTANCE = 0.0075f;
		float energy = this->team.getParameters().getEnergy();
		if (this->distanceTo(ball.getPosition()) > HIT_DISTANCE) {
			if (this->team.getNearestPlayer(ball.getPosition()) == &this->object) {
				this->chase(millis, ball.getPosition(), energy);
			} else {
				this->smartChase(millis, ball.getPosition(), energy);
			}
		} else {
			const int accuracy = 90;
			if (rand() % 101 < accuracy) {
				this->smartKick(ball, this->team.getKickDestination(), this->kickStrength);
			} else {
				this->smartKick(ball, this->team.getRandomPlayer().getPosition(), this->kickStrength);
			}
		}
	}
	
	AttackerControllerFactory::AttackerControllerFactory(TeamCollection &game, Team &team, float speed, float kickStrength)
		: PlayerControllerFactory(game, team, speed, kickStrength) {}
	
	std::unique_ptr<ObjectController> AttackerControllerFactory::create(ControllableObject &obj, KickHandler *kick) {
		std::unique_ptr<AttackerController> ctrl = std::make_unique<AttackerController>(obj, this->gameObjects, this->team, this->speed, this->kickStrength);
		ctrl->setKickHandler(kick);
		return ctrl;
	}
}