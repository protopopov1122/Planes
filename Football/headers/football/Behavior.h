#ifndef FOOTBALL_BEHAVIOR_H_
#define FOOTBALL_BEHAVIOR_H_

#include "football/GameObject.h"

namespace Football {

class BehaviorController : public ObjectController {
public:
  BehaviorController(ControllableObject &, GameObjectCollection &, float);
  void process(float) override;

protected:
  virtual void play(PassiveObject &, float) = 0;
  float distanceTo(Position &);
  bool chase(float, Position &, float = 1.0f, bool = true);
  void smartChase(float, Position &, float);
  void random(float, float, float, float);
  void generateRandom(float, float);
  bool move(Position);

  GameObjectCollection &gameObjects;
  float speed;
  Position randomPosition;
  Position smartChasePosition;
};
}

#endif