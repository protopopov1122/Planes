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

#include <algorithm>
#include "planes/PlaneSession.h"
#include "planes/PlanesApp.h"

namespace Planes {

  PlaneSession::PlaneSession() {
    this->opened = std::chrono::system_clock::now();
    this->sessionId = wxGetApp().getDatabase().openSession(*this);
  }

  PlaneSession::~PlaneSession() {
    wxGetApp().getDatabase().closeSession(this->sessionId);
  }

  std::chrono::system_clock::time_point PlaneSession::getOpenTime() const {
    return this->opened;
  }

  void PlaneSession::removePlanes(std::vector<Plane *> &rem) {
    this->planes.erase(std::remove_if(this->planes.begin(), this->planes.end(),
      [&](std::shared_ptr<Plane> ptr) { return std::find(rem.begin(), rem.end(), ptr.get()) != rem.end(); }), this->planes.end());
  }

  void PlaneSession::visit(PlaneSessionVisitor &v) {
    for (auto &plane : this->planes) {
      v.visit(*plane);
    }
  }

  Plane &PlaneSession::createPlane(std::string name, PlaneModel &model, PlaneMovementControllerFactory &movement) {
    std::shared_ptr<Plane> plane = std::make_shared<Plane>(this->sessionId, name, model);
    plane->setMovementController(movement.newController(*plane));
    this->planes.push_back(plane);
    return *plane;
  }
}