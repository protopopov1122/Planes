#ifndef FOOTBALL_GAME_OBJECT_H_
#define FOOTBALL_GAME_OBJECT_H_

#include "football/Controller.h"
#include "football/Drawable.h"
#include "football/base.h"
#include <memory>
#include <vector>

namespace Football {

class GameObject : public virtual ControllableObject {
public:
  GameObject(const Drawable &);
  virtual ~GameObject() = default;

  Drawable &getDrawable();
  ObjectController *getController();

  Position &getPosition() override;
  void setPosition(const Position &) override;
  float getRotation() const override;
  void setRotation(float) override;
  Rect getSize() override;

  static bool collision(ControllableObject &, ControllableObject &, Position &,
                        wxSize &);

protected:
  void setController(std::unique_ptr<ObjectController>);

private:
  Drawable drawable;
  Position position;
  std::unique_ptr<ObjectController> controller;
};

class GameVisitor {
public:
  virtual ~GameVisitor() = default;
  virtual void visit(GameObject &o) = 0;
};

class GameObjectCollection {
public:
  virtual ~GameObjectCollection() = default;
  virtual void visit(GameVisitor &, bool = true) = 0;
  virtual void visit(GameVisitor &, Position &, float, bool = true) = 0;
  virtual PassiveObject *getBall() = 0;
  virtual bool collision(Position &, ControllableObject &, wxSize &) = 0;
};
}

#endif