#include "football/GameObject.h"
#include <iostream>

namespace Football {

GameObject::GameObject(const Drawable &drawable)
    : drawable(drawable), position(0.5f, 0.5f) {}

Drawable &GameObject::getDrawable() { return this->drawable; }

void GameObject::setController(std::unique_ptr<ObjectController> obj) {
  this->controller = std::move(obj);
}

ObjectController *GameObject::getController() { return this->controller.get(); }

Position &GameObject::getPosition() { return this->position; }

void GameObject::setPosition(const Position &pos) { this->position = pos; }

float GameObject::getRotation() const { return this->drawable.getRotation(); }

void GameObject::setRotation(float rot) { this->drawable.setRotation(rot); }

Rect GameObject::getSize() {
  wxSize &size = this->drawable.getSize();
  Rect sz(0, 0, size.x, size.y);
  return sz;
}

bool GameObject::collision(ControllableObject &obj, ControllableObject &obj2,
                           Position &pos, wxSize &size) {
  if (&obj == &obj2) {
    return false;
  }
  float SCALE = 2;
  wxRect rect(static_cast<int>(pos.x * size.x - obj.getSize().w / SCALE),
              static_cast<int>(pos.y * size.y - obj.getSize().h / SCALE),
              static_cast<int>(obj.getSize().w / (0.5 * SCALE)),
              static_cast<int>(obj.getSize().h / (0.5 * SCALE)));
  wxRect rect2(static_cast<int>(obj2.getPosition().x * size.x -
                                obj2.getSize().w / SCALE),
               static_cast<int>(obj2.getPosition().y * size.y -
                                obj2.getSize().h / SCALE),
               static_cast<int>(obj2.getSize().w / (0.5 * SCALE)),
               static_cast<int>(obj2.getSize().h / (0.5 * SCALE)));
  return rect.Intersects(rect2);
}
}