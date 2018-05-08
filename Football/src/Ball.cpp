#include "football/Ball.h"

namespace Football {

	BallController::BallController(ControllableObject &obj, float slowDown, Rect &rect)
		: ObjectController::ObjectController(obj), field(rect), speed(0.0f), slowDown(slowDown) {}
	
	void BallController::process(float millis) {
		Position &pos = this->object.getPosition();
		const float rotation = this->object.getRotation();
		pos.x -= millis / 1000 * cos(rotation) * this->speed;
		pos.y -= millis / 1000 * sin(rotation) * this->speed;
		this->detectCollisions();
		this->speed = this->speed * (1 - this->slowDown * millis / 1000);
	}
	
	float BallController::getSpeed() const {
		return this->speed;
	}
	
	void BallController::setSpeed(float speed) {
		this->speed = speed;
	}
	
	void BallController::detectCollisions() {
		const float MIN_X = this->field.x;
		const float MAX_X = this->field.x + this->field.w;
		const float MIN_Y = this->field.y;
		const float MAX_Y = this->field.y + this->field.h;
		const float MIN_OFFSET = 1e-3;
		float rot = this->object.getRotation();
		Position position = this->object.getPosition();
		if (position.x <= MIN_X || position.x >= MAX_X ||
		  position.y <= MIN_Y || position.y >= MAX_Y) {
		  if (position.x <= MIN_X) {
			position.x = MIN_OFFSET + MIN_X;
			rot = M_PI - this->object.getRotation();
		  } else if (position.x >= MAX_X) {
			position.x = MAX_X - MIN_OFFSET;
			rot = M_PI - this->object.getRotation();
		  }
		  if (position.y <= MIN_Y) {
			position.y = MIN_OFFSET + MIN_Y;
			rot = -this->object.getRotation();
		  } else if (position.y >= MAX_Y) {
			position.y = MAX_Y - MIN_OFFSET;
			rot = -this->object.getRotation();
		  }
		  this->object.setRotation(rot);
		  this->object.setPosition(position);
	  }
	}
	
	BallControllerFactory::BallControllerFactory(float slowDown, Rect rect) : field(rect), slowDown(slowDown) {}
	
	std::unique_ptr<ObjectController> BallControllerFactory::create(ControllableObject &obj) {
		return std::make_unique<BallController>(obj, slowDown, this->field);
	}
	
	Ball::Ball(Drawable &draw, BallControllerFactory &fact)
		: GameObject::GameObject(draw) {
		this->setController(fact.create(*this));
	}
	
	BallController &Ball::getBallController() {
		return *(dynamic_cast<BallController *>(this->getController()));
	}
	
	float Ball::getSpeed() {
		return this->getBallController().getSpeed();
	}
	
	void Ball::setSpeed(float speed) {
		this->getBallController().setSpeed(speed);
	}
}