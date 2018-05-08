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

#ifndef PLANES_PLANES_APP_H_
#define PLANES_PLANES_APP_H_

#include "planes/base.h"
#include "planes/PlaneSession.h"
#include "planes/Database.h"
#include "planes/ResourceManager.h"

namespace Planes {

  class PlanesApp : public wxApp {
    public:
      bool OnInit() override;
      int OnExit() override;

      PlaneSession &getSession();
      PlaneDatabase &getDatabase();
      PlaneModelFactory &getModelFactory();
      ResourceManager &getResourceManager();
    private:
      std::unique_ptr<ResourceManager> resourceManager;
      std::unique_ptr<PlaneSession> session;
      std::unique_ptr<PlaneDatabase> database;
      PlaneModelFactory modelFactory;
  };
}

wxDECLARE_APP(Planes::PlanesApp);

#endif