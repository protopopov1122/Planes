#include "football/Team.h"
#include <cmath>
#include <iostream>

namespace Football {

	GoalkeeperController::GoalkeeperController(ControllableObject &obj, TeamCollection &game, Team &team, float speed, float kickStrength, float range)
		: PlayerController::PlayerController(obj, game, team, speed, kickStrength), range(range) {}
	
	void GoalkeeperController::play(PassiveObject &ball, float millis) {
		float energy = this->team.getParameters().getEnergy();
		Position &ballPosition = ball.getPosition();
		Position &dest = this->team.getKickDestination();
		Position point(1 - dest.x, dest.y);
		const float RADIUS = this->range;
		float dx = -point.x + ballPosition.x;
		float dy = -point.y + ballPosition.y;
		float hyp = hypot(dx, dy);
		if (hyp <= RADIUS) {
			const float HIT_DISTANCE = 0.01f;
			if (this->distanceTo(ball.getPosition()) > HIT_DISTANCE) {
				this->chase(millis, ball.getPosition(), energy);
			} else {
				this->smartKick(ball, dest, this->kickStrength);
			}
		} else {
			float angle = atan2(dy, dx);
			Position next(point.x + RADIUS * cos(angle), point.y + RADIUS * sin(angle));
			this->chase(millis, next, energy);
		}
	}
	
	GoalkeeperControllerFactory::GoalkeeperControllerFactory(TeamCollection &game, Team &team, float speed, float kickStrength, float range)
		: PlayerControllerFactory::PlayerControllerFactory(game, team, speed, kickStrength), range(range) {}
	
	std::unique_ptr<ObjectController> GoalkeeperControllerFactory::create(ControllableObject &obj, KickHandler *kick) {
		std::unique_ptr<GoalkeeperController> ctrl = std::make_unique<GoalkeeperController>(obj, this->gameObjects, this->team, this->speed, this->kickStrength, this->range);
		ctrl->setKickHandler(kick);
		return ctrl;
	}
}