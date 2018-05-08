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

#include <iostream>
#include <ctime>
#include <wx/msgdlg.h>
#include "planes/PlanesApp.h"
#include "planes/PlanesFrame.h"

namespace Planes {

    bool PlanesApp::OnInit() {
        std::string path;
        std::string dbpath;
        if (this->argc > 2) {
            path = std::string(this->argv[1]);
            dbpath = std::string(this->argv[2]);
        } else if (this->argc > 1) {
            path = std::string(this->argv[1]);
            wxFileDialog openFileDialog(nullptr, _("Select database file"));
            if (openFileDialog.ShowModal() != wxID_OK) {
                return false;
            }
            dbpath = openFileDialog.GetPath();
        } else {
            wxDirDialog openDirDialog(nullptr, _("Select data directory"));
            if (openDirDialog.ShowModal() != wxID_OK) {
                return false;
            }
            path = openDirDialog.GetPath();
            wxFileDialog openFileDialog(nullptr, _("Select database file"));
            if (openFileDialog.ShowModal() != wxID_OK) {
                return false;
            }
            dbpath = openFileDialog.GetPath();
        }

        this->resourceManager = std::make_unique<ResourceManager>(path);
        this->database = std::make_unique<PlaneDatabase>(dbpath);
        this->session = std::make_unique<PlaneSession>();
        // Define one or more planes this way
        // this->getModelFactory().createPlaneModel("Mercury", this->getResourceManager().loadBitmap("plane1.png"), wxSize(32, 32), 1.0f);
        // this->getModelFactory().createPlaneModel("Venus", this->getResourceManager().loadBitmap("plane2.png"), wxSize(32, 32), 0.5f);
        // this->getModelFactory().createPlaneModel("Moon", this->getResourceManager().loadBitmap("plane3.png"), wxSize(32, 32), 0.75f);
        // this->getModelFactory().createPlaneModel("Mars", this->getResourceManager().loadBitmap("plane4.png"), wxSize(32, 32), 0.25f);
        // this->getModelFactory().createPlaneModel("Jupiter", this->getResourceManager().loadBitmap("plane5.png"), wxSize(32, 32), 0.9f);
        // this->getModelFactory().createPlaneModel("Saturn", this->getResourceManager().loadBitmap("plane6.png"), wxSize(32, 32), 0.45f);

        // Uncomment this lines and put map file name there
        // PlanesFrame *frame = new PlanesFrame("map.png");
        // frame->Show(true);
        return true;
    }

    int PlanesApp::OnExit() {
        this->session.reset();
        return 0;
    }

    PlaneSession &PlanesApp::getSession() {
        return *this->session;
    }

    PlaneDatabase &PlanesApp::getDatabase() {
        return *this->database;
    }

    PlaneModelFactory &PlanesApp::getModelFactory() {
        return this->modelFactory;
    }

    ResourceManager &PlanesApp::getResourceManager() {
        return *this->resourceManager;
    }
}

wxIMPLEMENT_APP(Planes::PlanesApp);