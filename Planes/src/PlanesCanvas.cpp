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

#include "planes/PlanesCanvas.h"
#include "planes/PlaneSession.h"
#include "planes/PlanesApp.h"
#include <wx/dcbuffer.h>

namespace Planes {

  class CanvasRenderTimer : public wxTimer {
    public:
      CanvasRenderTimer(wxWindow *win) : win(win) {}
      void Notify() override {
        win->Refresh();
      }
    private:
      wxWindow *win;
  };

  class CanvasSessionRender : public PlaneSessionVisitor {
    public:
      CanvasSessionRender(wxDC &dc, wxSize &size) : dc(dc), canvasSize(size) {}
      void visit(Plane &plane) override {
        int x = plane.getPosition().x * this->canvasSize.GetX();
        int y = plane.getPosition().y * this->canvasSize.GetY();
        x -= plane.getModel().getVisualSize().GetX() / 2;
        y -= plane.getModel().getVisualSize().GetY() / 2;
        this->dc.DrawBitmap(plane.getBitmap(), x, y);
      }
    private:
      wxDC &dc;
      wxSize &canvasSize;
  };

  class SessionMovement : public PlaneSessionVisitor {
    public:
      SessionMovement(const float millis) : millis(millis) {}
      void visit(Plane &plane) override {
        if (auto ctrl = plane.getMovementController()) {
          ctrl->update(millis);
        }
      }
    private:
      const float millis;
  };

  class PlaneRemover : public PlaneSessionVisitor {
    public:
      PlaneRemover(std::vector<Plane *> &planes, wxSize &size, PlanePosition &pos)
        : planes(planes), canvasSize(size), mousePos(pos) {}
      void visit(Plane &plane) override {
        const wxSize &planeSize = plane.getModel().getVisualSize();
        PlanePosition pos = plane.getPosition();
        if (Plane::checkPlaneCollision(this->canvasSize, this->mousePos, planeSize, pos)) {
          this->planes.push_back(&plane);
        }
      }
    private:
      std::vector<Plane *> &planes;
      wxSize &canvasSize;
      PlanePosition &mousePos;
  };

  PlanesCanvas::PlanesCanvas(wxWindow *parent, wxWindowID id, int8_t fps, std::string &background)
    : wxWindow::wxWindow(parent, id), background() {
    this->background = wxGetApp().getResourceManager().loadBitmap(background);
    this->lastClock = 0;
    this->newPlaneModel = nullptr;
    this->newPlaneName = "";
	
	this->SetBackgroundStyle(wxBG_STYLE_PAINT);

    this->Bind(wxEVT_MOTION, &PlanesCanvas::OnMouseEvent, this);
    this->Bind(wxEVT_MOUSEWHEEL, &PlanesCanvas::OnMouseEvent, this);
    this->Bind(wxEVT_LEFT_DOWN, &PlanesCanvas::OnMouseClick, this);
		this->Bind(wxEVT_PAINT, &PlanesCanvas::OnPaintEvent, this);
    this->Bind(wxEVT_CLOSE_WINDOW, &PlanesCanvas::OnClose, this);

    this->timer = new CanvasRenderTimer(this);
    this->timer->Start(1000 / fps);
  }

  void PlanesCanvas::setNewPlaneName(std::string name) {
    this->newPlaneName = name;
  }

  void PlanesCanvas::createPlaneMode(PlaneModel *mod) {
    this->newPlaneName = "";
    if (mod != nullptr) {
      this->newPlaneRotation = 0.0f;
      this->newPlaneBitmap = mod->getBitmap();
    }
    this->newPlaneModel = mod;
  }

  void PlanesCanvas::OnClose(wxCloseEvent &evt) {
    this->timer->Stop();
    delete this->timer;
  }

  void PlanesCanvas::OnPaintEvent(wxPaintEvent &evt) {
    wxAutoBufferedPaintDC  dc(this);
    this->render(dc);
  }

  void PlanesCanvas::OnMouseEvent(wxMouseEvent &evt) {
    if (this->newPlaneModel != nullptr) {
      this->newPlanePosition = evt.GetPosition();
      if (evt.GetWheelRotation() != 0) {
        int direction = evt.GetWheelRotation() > 0 ? 1 : -1;
        this->newPlaneRotation += 6.28 * 5e-3 * direction;
        wxImage img = this->newPlaneModel->getBitmap().ConvertToImage();
        wxBitmap bitmap(img.Rotate(this->newPlaneRotation, wxPoint(img.GetWidth() / 2, img.GetHeight() / 2)));
        this->newPlaneBitmap = bitmap;
      }
    }
  }

  void PlanesCanvas::OnMouseClick(wxMouseEvent &evt) {
    wxSize canvasSize = this->GetSize();
    if (this->newPlaneModel != nullptr) {
      PlanePosition position(
        static_cast<float>(this->newPlanePosition.x) / canvasSize.GetX(),
        static_cast<float>(this->newPlanePosition.y) / canvasSize.GetY()
      );
      Plane &plane = wxGetApp().getSession().createPlane(this->newPlaneName, *this->newPlaneModel, PlaneMovementControllerFactory::Basic);
      plane.setPosition(position);
      plane.setRotation(this->newPlaneRotation);
    } else {
      wxPoint pos = evt.GetPosition();
      float x = static_cast<float>(pos.x) / canvasSize.GetX();
      float y = static_cast<float>(pos.y) / canvasSize.GetY();
      this->planeRemove(x, y);
    }
  }

  void PlanesCanvas::planeRemove(float x, float y) {
    wxSize canvasSize = this->GetSize();
    PlanePosition mousePosition(x, y);
    std::vector<Plane *> planes;
    PlaneRemover remover(planes, canvasSize, mousePosition);
    wxGetApp().getSession().visit(remover);
    wxGetApp().getSession().removePlanes(planes);
  }

  void PlanesCanvas::render(wxDC &dc) {
    float millis = 0.0f;
    if (this->lastClock != 0) {
      millis = static_cast<float>(clock() - this->lastClock) / (CLOCKS_PER_SEC / 1000);
    }
    this->lastClock = clock();
    if (millis != 0.0f) {
      SessionMovement mov(millis);
      wxGetApp().getSession().visit(mov);
    }

    wxSize canvasSize = this->GetSize();

    wxSize backgroundSize = this->background.GetSize();
    float scaleX = static_cast<float>(canvasSize.GetX()) / backgroundSize.GetX();
    float scaleY = static_cast<float>(canvasSize.GetY()) / backgroundSize.GetY();
    dc.SetUserScale(scaleX, scaleY);
    dc.DrawBitmap(this->background, 0, 0);
    dc.SetUserScale(1, 1);

    CanvasSessionRender sessionRender(dc, canvasSize);
    wxGetApp().getSession().visit(sessionRender);

    if (this->newPlaneModel != nullptr) {
      wxSize sz = this->newPlaneBitmap.GetSize();
      dc.DrawBitmap(this->newPlaneBitmap, this->newPlanePosition.x - sz.x / 2, this->newPlanePosition.y - sz.y / 2);
    }
  }
}