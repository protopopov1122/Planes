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

namespace Planes {

  PlaneModel::PlaneModel(std::string name, wxBitmap &orig, wxSize visualSize, float speed) : name(name), size(visualSize), speed(speed) {
    wxImage img = orig.ConvertToImage();
    img.Rescale(visualSize.GetX(), visualSize.GetY());
    this->bitmap = std::make_unique<wxBitmap>(img);
  }

  std::string PlaneModel::getName() const {
    return this->name;
  }

  wxBitmap &PlaneModel::getBitmap() const {
    return *this->bitmap;
  }

  const wxSize &PlaneModel::getVisualSize() const {
    return this->size;
  }
  
  float PlaneModel::getSpeed() const {
	return this->speed;
  }
  
  PlaneModel &PlaneModelFactory::createPlaneModel(std::string name, wxBitmap bitmap, wxSize vsize, float speed) {
    std::unique_ptr<PlaneModel> model = std::make_unique<PlaneModel>(name, bitmap, vsize, speed);
    PlaneModel &ref = *model;
    this->models.push_back(std::move(model));
    return ref;
  }

  void PlaneModelFactory::getModels(std::vector<PlaneModel *> &vec) {
    for (size_t i = 0; i < this->models.size(); i++) {
      vec.push_back(this->models.at(i).get());
    }
  }
}