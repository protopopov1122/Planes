#include "football/Referee.h"
#include <cmath>
#include <iostream>

namespace Football {

LinearRefereeController::LinearRefereeController(ControllableObject &obj,
                                                 GameObjectCollection &game,
                                                 float speed, float ln)
    : RefereeController::RefereeController(obj, game, speed), line(ln) {}

void LinearRefereeController::reset(PassiveObject &ball) {
  this->object.setPosition(Position(ball.getPosition().x, this->line));
}

void LinearRefereeController::play(PassiveObject &ball, float millis) {
  Position next(ball.getPosition().x, this->line);
  this->chase(millis, next, 0.5f, false);
}

LinearRefereeControllerFactory::LinearRefereeControllerFactory(
    GameObjectCollection &game, float speed, float line)
    : RefereeControllerFactory::RefereeControllerFactory(game, speed),
      line(line) {}

std::unique_ptr<ObjectController>
LinearRefereeControllerFactory::create(ControllableObject &obj) {
  return std::make_unique<LinearRefereeController>(obj, this->gameObjects,
                                                   this->speed, this->line);
}

void LinearRefereeControllerFactory::setLine(float l) { this->line = l; }
}