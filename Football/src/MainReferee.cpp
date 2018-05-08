#include "football/Referee.h"
#include <cmath>
#include <iostream>

namespace Football {

	MainRefereeController::MainRefereeController(ControllableObject &obj, GameObjectCollection &game, float speed, float initOffsetChangeInterval, float radius)
		: RefereeController::RefereeController(obj, game, speed), chaseOffset(0.0f, 0.0f), initOffsetChangeInterval(initOffsetChangeInterval), radius(radius) {}
	
	void MainRefereeController::reset(PassiveObject &ball) {
		this->object.setPosition(ball.getPosition());
	}
	
	void MainRefereeController::play(PassiveObject &ball, float millis) {
		this->checkChaseOffset();
		Position dest(ball.getPosition().x + this->chaseOffset.x,
			ball.getPosition().y + this->chaseOffset.y);
		if (!this->chase(millis, dest) || this->offsetChangeInterval <= 0.0f) {
			newChaseOffset();
		}
		this->offsetChangeInterval -= millis;
	}
	
	void MainRefereeController::checkChaseOffset() {
		const float RANGE = 0.01f;
		if ((fabs(this->chaseOffset.x) <= RANGE &&
			fabs(this->chaseOffset.y) <= RANGE)) {
			newChaseOffset();
		}
	}
	
	void MainRefereeController::newChaseOffset() {
		const float MIN_RADIUS = this->radius;
		float radius = MIN_RADIUS + MIN_RADIUS / (rand() % 5 + 1);
		float angle = 2 * M_PI / (rand() % 5 + 1);
		this->chaseOffset = Position(sin(angle) * radius, cos(angle) * radius);
		this->offsetChangeInterval = this->initOffsetChangeInterval;
	}
	
	MainRefereeControllerFactory::MainRefereeControllerFactory(GameObjectCollection &game, float speed, float initOffsetChangeInterval, float radius)
		: RefereeControllerFactory::RefereeControllerFactory(game, speed), initOffsetChangeInterval(initOffsetChangeInterval), radius(radius) {}
	
	std::unique_ptr<ObjectController> MainRefereeControllerFactory::create(ControllableObject &obj) {
		return std::make_unique<MainRefereeController>(obj, this->gameObjects, this->speed, this->initOffsetChangeInterval, this->radius);
	}
}