#include "football/Game.h"

namespace Football {

class GameController : public GameVisitor {
public:
  GameController(float millis) : millis(millis) {}
  void visit(GameObject &obj) override {
    ObjectController *ctrl = obj.getController();
    if (ctrl) {
      ctrl->process(this->millis);
    }
  }

private:
  float millis;
};

Game::Game()
    : kickHandler(nullptr), speed(1.0f), ball(nullptr),
      lastKick(std::make_pair(nullptr, nullptr)) {}

void Game::setKickHandler(KickHandler &h) { this->kickHandler = &h; }

void Game::setTeams(std::unique_ptr<Team> t1, std::unique_ptr<Team> t2) {
  this->teams = std::make_pair(std::move(t1), std::move(t2));
}

std::pair<Team *, Team *> Game::getTeams() {
  return std::make_pair(this->teams.first.get(), this->teams.second.get());
}

void Game::addReferee(std::unique_ptr<Referee> referee) {
  this->referees.push_back(std::move(referee));
}

void Game::round() {
  this->lastKick = std::make_pair(nullptr, nullptr);
  this->teams.first->spawn(true);
  this->teams.second->spawn(true);
  Position center(0.5f, 0.5f);
  this->ball->setPosition(center);
  this->ball->getBallController().setSpeed(0.0f);
  for (std::size_t i = 0; i < this->referees.size(); i++) {
    this->referees.at(i)->reset(*this->ball);
  }
}

void Game::visit(GameVisitor &v, bool includeBall) {
  if (this->ball.get() != nullptr && includeBall) {
    v.visit(*this->ball);
  }
  if (this->teams.first != nullptr) {
    this->teams.first->visit(v);
  }
  if (this->teams.second != nullptr) {
    this->teams.second->visit(v);
  }
  for (std::size_t i = 0; i < this->referees.size(); i++) {
    v.visit(*this->referees.at(i));
  }
}

void Game::visit(GameVisitor &v, Position &center, float radius,
                 bool includeBall) {
  if (this->ball.get() != nullptr && includeBall) {
    float dx = this->ball->getPosition().x - center.x;
    float dy = this->ball->getPosition().y - center.y;
    if (hypot(dx, dy) <= radius) {
      v.visit(*this->ball);
    }
  }
  if (this->teams.first != nullptr) {
    this->teams.first->visit(v, center, radius);
  }
  if (this->teams.second != nullptr) {
    this->teams.second->visit(v, center, radius);
  }
  for (std::size_t i = 0; i < this->referees.size(); i++) {
    GameObject &obj = *this->referees.at(i);
    float dx = center.x - obj.getPosition().x;
    float dy = center.y - obj.getPosition().y;
    if (hypot(dx, dy) <= radius) {
      v.visit(obj);
    }
  }
}

Ball *Game::getBall() { return this->ball.get(); }

void Game::setBall(std::unique_ptr<Ball> ball) { this->ball = std::move(ball); }

bool Game::collision(Position &pos, ControllableObject &obj, wxSize &sz) {
  if ((this->teams.first != nullptr &&
       this->teams.first->playerCollides(obj, pos, sz)) ||
      (this->teams.second != nullptr &&
       this->teams.second->playerCollides(obj, pos, sz))) {
    return true;
  }
  for (std::size_t i = 0; i < this->referees.size(); i++) {
    if (GameObject::collision(obj, *this->referees.at(i), pos, sz)) {
      return true;
    }
  }
  return false;
}

std::vector<std::pair<size_t, size_t>> &Game::getGoals() { return this->goals; }

bool Game::checkGoals() {
  Position &ballPosition = this->ball->getPosition();
  if (this->checkGoal(this->teams.first->getGoal(), ballPosition)) {
    this->teams.second->getParameters().incScore();
    if (this->lastKick.first != nullptr && this->lastKick.second != nullptr) {
      this->goals.push_back(std::make_pair(this->lastKick.first->getID(),
                                           this->lastKick.second->getID()));
    }
    return true;
  } else if (this->checkGoal(this->teams.second->getGoal(), ballPosition)) {
    this->teams.first->getParameters().incScore();
    if (this->lastKick.first != nullptr && this->lastKick.second != nullptr) {
      this->goals.push_back(std::make_pair(this->lastKick.first->getID(),
                                           this->lastKick.second->getID()));
    }
    return true;
  }

  return false;
}

bool Game::checkGoal(Rect &goal, Position &pos) {
  return pos.x >= goal.x - goal.w / 2 && pos.y >= goal.y &&
         pos.x <= goal.x + goal.w * 1.5f && pos.y <= goal.y + goal.h;
}

std::pair<uint16_t, uint16_t> Game::getScore() {
  return std::make_pair(this->teams.first->getParameters().getScore(),
                        this->teams.second->getParameters().getScore());
}

bool Game::process(float millis) {
  if (this->speed <= 1.0f) {
    GameController ctrl(millis * this->speed);
    this->visit(ctrl);
  } else {
    GameController ctrl(millis);
    for (float f = 0.0f; f < this->speed; f += 1.0f) {
      this->visit(ctrl);
    }
  }
  return this->checkGoals();
}

void Game::setSpeed(float speed) { this->speed = speed; }

std::pair<Team *, Player *> Game::getLastKick() { return this->lastKick; }

void Game::registerKick(Team &team, Player &player) {
  this->lastKick = std::make_pair(&team, &player);
  if (this->kickHandler) {
    this->kickHandler->registerKick();
  }
}
}