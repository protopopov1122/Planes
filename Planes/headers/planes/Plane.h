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

#ifndef PLANES_PLANE_H_
#define PLANES_PLANE_H_

#include <chrono>
#include <memory>
#include "planes/base.h"
#include <wx/bitmap.h>
#include "planes/PlaneMovement.h"
#include "planes/PlaneModel.h"

namespace Planes {

  struct PlanePosition {
    PlanePosition() : x(0.0f), y(0.0f) {}
    PlanePosition(float x, float y) : x(x), y(y) {}
    PlanePosition(const PlanePosition &pos) : x(pos.x), y(pos.y) {}
    float x;
    float y;
  };

  class Plane {
    public:
      Plane(int32_t, std::string, PlaneModel &);
      virtual ~Plane();

      std::string getName() const;
      PlaneModel &getModel() const;
      wxBitmap &getBitmap();
      std::chrono::system_clock::time_point getCreateTime() const;
      PlanePosition &getPosition();
      void setPosition(const PlanePosition &);
      float getRotation() const;
      void setRotation(float);
      void setMovementController(std::unique_ptr<PlaneMovementController>);
      PlaneMovementController *getMovementController();

      static bool checkPlaneCollision(const wxSize &, const PlanePosition &, const wxSize &, const PlanePosition &, const wxSize &);
      static bool checkPlaneCollision(const wxSize &, const PlanePosition &, const wxSize &, const PlanePosition &);
    private:
      void updateBitmap();

      std::unique_ptr<PlaneMovementController> movement;
      int64_t planeId;
      std::string name;
      PlaneModel &model;
      std::chrono::system_clock::time_point created;
      PlanePosition position;
      float rotation;
      wxBitmap bitmap;
  };
}

#endif