/*
  MIT License

  Copyright (c) 2018 Jevgēnijs Protopopovs

  Permission is hereby granted, free of charge, to any person obtaining a copy
  of this software and associated documentation files (the "Software"), to deal
  in the Software without restriction, including without limitation the rights
  to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
  copies of the Software, and to permit persons to whom the Software is
  furnished to do so, subject to the following conditions:

  The above copyright notice and this permission notice shall be included in all
  copies or substantial portions of the Software.

  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
  AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
  OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
  SOFTWARE.
*/

#include <cmath>
#include "planes/PlaneMovement.h"
#include "planes/Plane.h"

namespace Planes {

  PlaneMovementController::PlaneMovementController(Plane &plane) : plane(plane) {}

  PlaneBasicMovementController::PlaneBasicMovementController(Plane &plane)
    : PlaneMovementController::PlaneMovementController(plane) {}

  void PlaneBasicMovementController::update(const float millis) {
    PlanePosition &pos = this->plane.getPosition();
    const float rotation = this->plane.getRotation();
	const float speed = this->plane.getModel().getSpeed();
    pos.x -= millis / 1000 * sin(rotation) / 5 * speed;
    pos.y -= millis / 1000 * cos(rotation) / 5 * speed;
    this->detectCollisions();
  }

  void PlaneBasicMovementController::detectCollisions() {
    const float MIN_OFFSET = 1e-3;
    float rot = this->plane.getRotation();
    PlanePosition position = this->plane.getPosition();
    if (position.x <= 0.0f || position.x >= 1.0f ||
      position.y <= 0.0f || position.y >= 1.0f) {
      if (position.x <= 0.0f) {
        position.x = MIN_OFFSET;
        float angle = this->plane.getRotation() - M_PI / 2;
        rot = 3 * M_PI / 2 - angle;
      } else if (position.x >= 1.0f) {
        position.x = 1.0f - MIN_OFFSET;
        float angle = this->plane.getRotation() - 3 * M_PI / 2;
        rot = M_PI / 2 - angle;
      }
      if (position.y <= 0.0f) {
        position.y = MIN_OFFSET;
        float angle = this->plane.getRotation();
        if (angle >= M_PI) {
          angle -= 2 * M_PI;
        } 
        rot = M_PI - angle;
      } else if (position.y >= 1.0f) {
        position.y = 1.0f - MIN_OFFSET;
        float angle = this->plane.getRotation() - M_PI;
        rot = -angle;
      }
      this->plane.setRotation(rot);
      this->plane.setPosition(position);
    }
  }

  class BasicPlaneMovementControllerFactory : public PlaneMovementControllerFactory {
    public:
      std::unique_ptr<PlaneMovementController> newController(Plane &plane) override {
        return std::make_unique<PlaneBasicMovementController>(plane);
      }
  };

  static BasicPlaneMovementControllerFactory basic;
  PlaneMovementControllerFactory &PlaneMovementControllerFactory::Basic = basic;
}