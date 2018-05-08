#include "football/Team.h"
#include <cmath>

namespace Football {

class KickCorrection : public PlayerVisitor {
public:
  KickCorrection(Team &team, Position &pos, float angle)
      : team(team), ppos(pos) {
    this->dx = cos(angle);
    this->dy = sin(angle);
  }
  void visit(Player &pl) override {
    if (&this->team != &pl.getTeam()) {
      Position &opos = pl.getPosition();
      float distance = hypot(ppos.x - opos.x, ppos.y - opos.y) + 0.5f;
      float angle = atan2(ppos.y - opos.y, ppos.x - opos.x);
      dx += cos(angle) / distance;
      dy += sin(angle) / distance;
    }
  }
  float getAngle() const { return atan2(this->dy, this->dx); }

private:
  Team &team;
  Position &ppos;
  float dx;
  float dy;
};

PlayerController::PlayerController(ControllableObject &obj,
                                   TeamCollection &game, Team &team,
                                   float speed, float kickStrength)
    : BehaviorController::BehaviorController(obj, game, speed), teams(game),
      team(team), kickHandler(nullptr), kickStrength(kickStrength) {}

Team &PlayerController::getTeam() { return this->team; }

void PlayerController::setKickHandler(KickHandler *handler) {
  this->kickHandler = handler;
}

float PlayerController::kickCorrection(float angle) {
  KickCorrection corr(this->team, this->object.getPosition(), angle);
  std::pair<Team *, Team *> teams = this->teams.getTeams();
  Team *opposite = teams.first != &this->team ? teams.first : teams.second;
  if (opposite) {
    opposite->visit(corr, this->object.getPosition(), 0.05f);
  }
  return corr.getAngle();
}

void PlayerController::kick(PassiveObject &object, Position &destination,
                            float speed) {
  Position &position = object.getPosition();
  const float RANDOM_RANGE = M_PI / 64;
  float energy = this->team.getParameters().getEnergy() * 2;
  float dx = -destination.x + position.x;
  float dy = -destination.y + position.y;
  float angle = this->kickCorrection(atan2(dy, dx)) +
                RANDOM_RANGE * 2 / (rand() % 4 + 1) - RANDOM_RANGE;
  object.setRotation(angle);
  object.setSpeed(speed * energy);

  if (this->kickHandler) {
    this->kickHandler->registerKick();
  }
}

void PlayerController::smartKick(PassiveObject &object, Position &position,
                                 float speed) {
  GameObject *nearest = this->team.getNearestPlayer(position);
  if (&this->object == nearest) {
    this->kick(object, position, speed);
  } else {
    this->kick(object, nearest->getPosition(), speed);
  }
}

PlayerController &Player::getBehavior() {
  return *(dynamic_cast<PlayerController *>(this->getController()));
}

PlayerControllerFactory::PlayerControllerFactory(TeamCollection &gameObjects,
                                                 Team &team, float speed,
                                                 float kickStrength)
    : gameObjects(gameObjects), team(team), speed(speed),
      kickStrength(kickStrength) {}

Player::Player(Drawable &draw, Team &team, KickRegister &kickRegister,
               PlayerControllerFactory &fact, size_t id)
    : GameObject::GameObject(draw), team(team), kickRegister(kickRegister),
      enabled(true), id(id) {
  this->setController(fact.create(*this, this));
}

size_t Player::getID() { return this->id; }

void Player::registerKick() {
  this->kickRegister.registerKick(this->team, *this);
}

Team &Player::getTeam() { return this->getBehavior().getTeam(); }

bool Player::isEnabled() const { return this->enabled; }

void Player::enable(bool en) { this->enabled = en; }
}