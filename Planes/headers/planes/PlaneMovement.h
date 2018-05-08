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

#ifndef PLANES_PLANE_MOVEMENT_CONTROLLER_H_
#define PLANES_PLANE_MOVEMENT_CONTROLLER_H_

#include "planes/base.h"
#include <memory>

namespace Planes {

  class Plane; // Forward referencing

  class PlaneMovementController {
    public:
      PlaneMovementController(Plane &);
      virtual ~PlaneMovementController() = default;
      virtual void update(const float) = 0;
    protected:
      Plane &plane;
  };

  class PlaneBasicMovementController : public PlaneMovementController {
    public:
      PlaneBasicMovementController(Plane &);
      void update(const float) override;
    private:
      void detectCollisions();
  };

  class PlaneMovementControllerFactory {
    public:
      virtual ~PlaneMovementControllerFactory() = default;
      virtual std::unique_ptr<PlaneMovementController> newController(Plane &) = 0;

      static PlaneMovementControllerFactory &Basic;
  };
}

#endif