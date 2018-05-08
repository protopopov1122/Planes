#ifndef FOOTBALL_CONTROLLABLE_OBJECT_H_
#define FOOTBALL_CONTROLLABLE_OBJECT_H_

#include "football/base.h"

namespace Football {

class ControllableObject {
public:
  virtual ~ControllableObject() = default;
  virtual Position &getPosition() = 0;
  virtual void setPosition(const Position &) = 0;
  virtual float getRotation() const = 0;
  virtual void setRotation(float) = 0;
  virtual Rect getSize() = 0;
};

class PassiveObject : public virtual ControllableObject {
public:
  virtual float getSpeed() = 0;
  virtual void setSpeed(float) = 0;
};
}

#endif