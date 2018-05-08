#include "football/Team.h"
#include <cmath>
#include <iostream>

namespace Football {

DefenderController::DefenderController(ControllableObject &obj,
                                       TeamCollection &game, Team &team,
                                       float speed, float kickStrength)
    : PlayerController::PlayerController(obj, game, team, speed, kickStrength) {
}

void DefenderController::play(PassiveObject &ball, float millis) {
  float energy = this->team.getParameters().getEnergy();
  Position &dest = this->team.getKickDestination();
  float delta = fabs(dest.x - ball.getPosition().x);
  if (delta < 0.5f) {
    this->random(millis, 1.0f - dest.x, fabs(dest.x - 0.5f), energy);
  } else {
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
  }
}

DefenderControllerFactory::DefenderControllerFactory(TeamCollection &game,
                                                     Team &team, float speed,
                                                     float kickStrength)
    : PlayerControllerFactory(game, team, speed, kickStrength) {}

std::unique_ptr<ObjectController>
DefenderControllerFactory::create(ControllableObject &obj, KickHandler *kick) {
  std::unique_ptr<DefenderController> ctrl =
      std::make_unique<DefenderController>(obj, this->gameObjects, this->team,
                                           this->speed, this->kickStrength);
  ctrl->setKickHandler(kick);
  return ctrl;
}
}