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

#ifndef PLANES_PLANE_MODEL_H_
#define PLANES_PLANE_MODEL_H_

#include <vector>
#include "planes/base.h"
#include <wx/bitmap.h>

namespace Planes {

  class Plane; // Forward referencing

  class PlaneModel {
    public:
      PlaneModel(std::string, wxBitmap &, wxSize, float);
      std::string getName() const;
      wxBitmap &getBitmap() const;
      const wxSize &getVisualSize() const;
	  float getSpeed() const;
    private:
      std::string name;
      std::unique_ptr<wxBitmap> bitmap;
      const wxSize size;
	  const float speed;
  };

  class PlaneModelFactory {
    public:
      PlaneModel &createPlaneModel(std::string, wxBitmap, wxSize, float);
      void getModels(std::vector<PlaneModel *> &);
    private:
      std::vector<std::unique_ptr<PlaneModel>> models;
  };
}

#endif