#ifndef FOOTBALL_CONTROLLER_H_
#define FOOTBALL_CONTROLLER_H_

#include "football/ControllableObject.h"
#include "football/base.h"
#include <memory>

namespace Football {

class ObjectController {
public:
  ObjectController(ControllableObject &obj) : object(obj) {}
  virtual ~ObjectController() = default;

  virtual void process(float) = 0;

protected:
  ControllableObject &object;
};
}

#endif