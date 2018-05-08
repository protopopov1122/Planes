#ifndef FOOTBALL_BALL_H_
#define FOOTBALL_BALL_H_

#include "football/GameObject.h"

namespace Football {

class BallController : public ObjectController {
public:
  BallController(ControllableObject &, float, Rect &);
  void process(float) override;
  float getSpeed() const;
  void setSpeed(float);

private:
  void detectCollisions();
  Rect field;
  float speed;
  float slowDown;
};

class BallControllerFactory {
public:
  BallControllerFactory(float, Rect);
  std::unique_ptr<ObjectController> create(ControllableObject &);

private:
  Rect field;
  float slowDown;
};

class Ball : public GameObject, public PassiveObject {
public:
  Ball(Drawable &, BallControllerFactory &);
  BallController &getBallController();
  float getSpeed() override;
  void setSpeed(float) override;
};
}

#endif