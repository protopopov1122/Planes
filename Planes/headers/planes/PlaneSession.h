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

#ifndef PLANES_PLANE_SESSION_H_
#define PLANES_PLANE_SESSION_H_

#include <vector>
#include <chrono>
#include <memory>
#include "planes/base.h"
#include "planes/Plane.h"

namespace Planes {

  class PlaneSessionVisitor {
    public:
      virtual ~PlaneSessionVisitor() = default;
      virtual void visit(Plane &) = 0;
  };

  class PlaneSession {
    public:
      PlaneSession();
      virtual ~PlaneSession();
      std::chrono::system_clock::time_point getOpenTime() const;

      Plane &createPlane(std::string, PlaneModel &, PlaneMovementControllerFactory &);
      void removePlanes(std::vector<Plane *> &);
      void visit(PlaneSessionVisitor &);
    private:
      int64_t sessionId;
      std::chrono::system_clock::time_point opened;
      std::vector<std::shared_ptr<Plane>> planes;
  };
}

#endif