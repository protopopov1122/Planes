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

#include "planes/Plane.h"
#include "planes/PlanesApp.h"

namespace Planes {

  Plane::Plane(int32_t sid, std::string name, PlaneModel &model) : name(name), model(model), rotation(0.0f) {
    this->created = std::chrono::system_clock::now();
    this->updateBitmap();
    this->planeId = wxGetApp().getDatabase().newPlane(sid, *this);
  }

  Plane::~Plane() {
    wxGetApp().getDatabase().destroyPlane(this->planeId);
  }

  std::string Plane::getName() const {
    return this->name;
  }

  PlaneModel &Plane::getModel() const {
    return this->model;
  }

  wxBitmap &Plane::getBitmap() {
    return this->bitmap;
  }

  std::chrono::system_clock::time_point Plane::getCreateTime() const {
    return this->created;
  }

  PlanePosition &Plane::getPosition() {
    return this->position;
  }

  void Plane::setPosition(const PlanePosition &pos) {
    this->position = pos;
  }

  float Plane::getRotation() const {
    return this->rotation;
  }

  void Plane::setRotation(float rot) {
    this->rotation = rot;
    while (this->rotation >= 2 * M_PI) {
      this->rotation -= 2 * M_PI;
    }
    while (this->rotation < 0) {
      this->rotation += 2 * M_PI;
    }
    updateBitmap();
  }

  void Plane::setMovementController(std::unique_ptr<PlaneMovementController> ctrl) {
    this->movement = std::move(ctrl);
  }

  PlaneMovementController *Plane::getMovementController() {
    return this->movement.get();
  }

  void Plane::updateBitmap() {
    wxImage img = this->getModel().getBitmap().ConvertToImage();
    wxBitmap bitmap(img.Rotate(this->getRotation(), wxPoint(img.GetWidth() / 2, img.GetHeight() / 2)));
    this->bitmap = bitmap;
  }

  bool Plane::checkPlaneCollision(const wxSize &screenSize,
    const PlanePosition &plane1Pos, const wxSize &plane1Size,
    const PlanePosition &plane2Pos, const wxSize &plane2Size) {

    wxRect plane1(plane1Pos.x * screenSize.x - plane1Size.x / 2,
                  plane1Pos.y * screenSize.y - plane1Size.y / 2,
                  plane1Size.x, plane1Size.y);
    wxRect plane2(plane2Pos.x * screenSize.x - plane2Size.x / 2,
                  plane2Pos.y * screenSize.y - plane2Size.y / 2,
                  plane2Size.x, plane2Size.y);
    return plane1.Intersects(plane2);
  }

  bool Plane::checkPlaneCollision(const wxSize &screenSize,
    const PlanePosition &plane1Pos, const wxSize &plane1Size,
    const PlanePosition &ppos) {

    wxPoint pos(ppos.x * screenSize.x, ppos.y * screenSize.y);
    wxRect plane1(plane1Pos.x * screenSize.x - plane1Size.x / 2,
                  plane1Pos.y * screenSize.y - plane1Size.y / 2,
                  plane1Size.x, plane1Size.y);
    return plane1.Contains(pos);
  }
}