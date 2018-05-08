#include "football/Referee.h"

namespace Football {

	RefereeControllerFactory::RefereeControllerFactory(GameObjectCollection &gameObjects, float speed)
		: gameObjects(gameObjects), speed(speed) {}
		
	Referee::Referee(Drawable &draw, RefereeControllerFactory &fact)
		: GameObject::GameObject(draw) {
		this->setController(fact.create(*this));
	}
	
	void Referee::reset(PassiveObject &ball) {
		this->getBehavior().reset(ball);
	}
	
	RefereeController::RefereeController(ControllableObject &obj, GameObjectCollection &game, float speed)
		: BehaviorController::BehaviorController(obj, game, speed) {}
	
	RefereeController &Referee::getBehavior() {
		return *(dynamic_cast<RefereeController *>(this->getController()));
	}
}