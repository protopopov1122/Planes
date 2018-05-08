#include "football/Behavior.h"

namespace Football {

class CollisionAvoider : public GameVisitor {
public:
  CollisionAvoider(ControllableObject &object, float angle) : object(object) {
    this->dx = cos(angle);
    this->dy = sin(angle);
  }
  void avoid(Position &opos) {
    Position &ppos = this->object.getPosition();
    float distance = pow(hypot(ppos.x - opos.x, ppos.y - opos.y), 2) * 500 + 5;
    float angle = atan2(ppos.y - opos.y, ppos.x - opos.x);
    dx += cos(angle) / distance;
    dy += sin(angle) / distance;
  }
  void visit(GameObject &obj) override {
    if (&this->object == &obj) {
      return;
    }
    Position &opos = obj.getPosition();
    this->avoid(opos);
  }
  float getAngle() const { return atan2(dy, dx); }
  float getMagnitude() const { return hypot(dx, dy); }

private:
  ControllableObject &object;
  float dx;
  float dy;
};

BehaviorController::BehaviorController(ControllableObject &obj,
                                       GameObjectCollection &gameObjects,
                                       float speed)
    : ObjectController::ObjectController(obj), gameObjects(gameObjects),
      speed(speed), randomPosition(0.0f, 0.0f), smartChasePosition(0.0f, 0.0f) {
}

void BehaviorController::process(float millis) {
  PassiveObject *ball = this->gameObjects.getBall();
  if (ball != nullptr) {
    this->play(*ball, millis);
  }
}

float BehaviorController::distanceTo(Position &position) {
  float dx = position.x - this->object.getPosition().x;
  float dy = position.y - this->object.getPosition().y;
  float hyp = sqrt(dx * dx + dy * dy);
  return hyp;
}

bool BehaviorController::move(Position pos) {
  wxSize sz = Drawable::getCanvasSize();
  if (pos.x > 1.0f || pos.x < 0.0f || pos.y > 1.0f || pos.y < 0.0f ||
      this->gameObjects.collision(pos, this->object, sz)) {
    return false;
  } else {
    this->object.setPosition(pos);
    return true;
  }
}

bool BehaviorController::chase(float millis, Position &destination,
                               float energy, bool avoidCollisions) {
  Position &position = this->object.getPosition();
  float hyp = hypot(destination.x - position.x, destination.y - position.y);
  float angle = atan2(destination.y - position.y, destination.x - position.x);
  if (avoidCollisions) {
    CollisionAvoider avoider(this->object, angle);
    this->gameObjects.visit(avoider, this->object.getPosition(), 0.1f, false);
    Position p1(this->object.getPosition().x, 0.0f);
    Position p2(this->object.getPosition().x, 1.0f);
    Position p3(0.0f, this->object.getPosition().y);
    Position p4(1.0f, this->object.getPosition().y);
    avoider.avoid(p1);
    avoider.avoid(p2);
    avoider.avoid(p3);
    avoider.avoid(p4);
    angle = avoider.getAngle();
  }
  float dx = cos(angle);
  float dy = sin(angle);
  float speed = this->speed * millis / 1000 * 2 * energy;
  const float BARRIER = 0.005f;
  if (hyp > BARRIER) {
    const float STEP = speed / 20;
    const float BARRIER = STEP * speed * 2;
    for (float i = 0; i < speed &&
                      hypot(destination.x - position.x,
                            destination.y - position.y) > BARRIER;
         i += STEP) {
      Position next(position.x + STEP * dx, position.y + STEP * dy);
      if (!this->move(next)) {
        float dx = 2 * speed / (rand() % 5 + 1) - speed;
        float dy = 2 * speed / (rand() % 5 + 1) - speed;
        return this->move(Position(next.x + dx, next.y + dy));
      }
    }
    return true;
  } else {
    return true;
  }
}

void BehaviorController::smartChase(float millis, Position &destination,
                                    float energy) {
  Position &curPos = this->object.getPosition();
  const float RANGE = 0.01f;
  const float MIN_RADIUS = 0.1f;
  if ((fabs(destination.x + this->smartChasePosition.x - curPos.x) <= RANGE &&
       fabs(destination.y + this->smartChasePosition.y - curPos.y) <= RANGE) ||
      (fabs(destination.x + this->smartChasePosition.x) <= RANGE &&
       fabs(destination.x + this->smartChasePosition.y) <= RANGE)) {
    float angle = M_PI * 2 / (rand() % 10 + 1);
    float radius = MIN_RADIUS + MIN_RADIUS / (rand() % 5 + 1);
    this->smartChasePosition =
        Position(radius * cos(angle), radius * sin(angle));
  }
  Position chasePos = Position(destination.x + this->smartChasePosition.x,
                               destination.y + this->smartChasePosition.y);
  if (!this->chase(millis, chasePos, energy)) {
    float angle = M_PI * 2 / (rand() % 10 + 1);
    float radius = MIN_RADIUS + MIN_RADIUS / (rand() % 5 + 1);
    this->smartChasePosition =
        Position(radius * cos(angle), radius * sin(angle));
  }
}

void BehaviorController::random(float millis, float from, float to,
                                float energy) {
  if (from > to) {
    float temp = to;
    to = from;
    from = temp;
  }
  this->generateRandom(from, to);
  if (!this->chase(millis, this->randomPosition, energy)) {
    this->randomPosition = Position(from + (to - from) / (rand() % 5 + 1),
                                    0.01f * (rand() % 100 + 1));
  }
}

void BehaviorController::generateRandom(float from, float to) {
  const float RANGE = 0.01f;
  Position &curPos = this->object.getPosition();
  if ((fabs(this->randomPosition.x - curPos.x) <= RANGE &&
       fabs(this->randomPosition.y - curPos.y) <= RANGE) ||
      (fabs(this->randomPosition.x) <= RANGE &&
       fabs(this->randomPosition.y) <= RANGE)) {
    this->randomPosition = Position(from + (to - from) / (rand() % 5 + 1),
                                    0.01f * (rand() % 100 + 1));
  }
}
}