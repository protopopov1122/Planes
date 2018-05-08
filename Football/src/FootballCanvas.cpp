#include "football/FootballCanvas.h"
#include "football/FootballApp.h"
#include "football/FootballFrame.h"
#include "football/GameObject.h"
#include <iostream>
#include <wx/dcbuffer.h>

namespace Football {

wxDEFINE_EVENT(FOOTBALL_STATUS_CHANGE, wxCommandEvent);
wxDEFINE_EVENT(FOOTBALL_SCORE_CHANGE, wxCommandEvent);
wxDEFINE_EVENT(FOOTBALL_ROUND, wxCommandEvent);

AudioThread::AudioThread()
    : wxThread::wxThread(wxTHREAD_JOINABLE), sound(nullptr), work(true),
      cond(mutex) {}

bool AudioThread::play(wxSound &snd) {
  if (this->sound != nullptr) {
    return false;
  } else {
    this->sound = &snd;
    wxMutexLocker lock(this->mutex);
    this->cond.Broadcast();
    return true;
  }
}

bool AudioThread::isPlaying() { return this->sound != nullptr; }

void AudioThread::exit() {
  wxSound::Stop();
  this->work = false;
  wxMutexLocker lock(this->mutex);
  this->cond.Broadcast();
}

void *AudioThread::Entry() {
  while (work) {
    while (this->sound == nullptr && work) {
      this->mutex.Lock();
      this->cond.Wait();
    }
    if (this->sound != nullptr) {
      this->sound->Play(wxSOUND_SYNC);
      this->sound = nullptr;
    }
  }
  return nullptr;
}

class CanvasRenderTimer : public wxTimer {
public:
  CanvasRenderTimer(wxWindow *win) : win(win) {}
  void Notify() override { win->Refresh(); }

private:
  wxWindow *win;
};

class CanvasGameRender : public GameVisitor {
public:
  CanvasGameRender(wxDC &dc, wxSize &sz) : dc(dc), size(sz) {}
  void visit(GameObject &obj) override {
    wxSize &sz = obj.getDrawable().getSize();
    Position &pos = obj.getPosition();
    wxBitmap &bitmap = obj.getDrawable().getBitmap();
    dc.DrawBitmap(bitmap, static_cast<wxCoord>(pos.x * this->size.x - sz.x / 2),
                  static_cast<wxCoord>(pos.y * this->size.y - sz.y / 2));
  }

private:
  wxDC &dc;
  wxSize size;
};

FootballCanvas::FootballCanvas(wxWindow *parent, wxWindowID id, int8_t fps,
                               std::string &background)
    : wxWindow::wxWindow(parent, id), background(), projector(5000.0f),
      fpsSum(0.0f), fpsCount(0) {
  this->background = wxGetApp().getResourceManager().loadBitmap(background);
  this->lastClock = 0;

  this->SetBackgroundStyle(wxBG_STYLE_PAINT);

  this->Bind(wxEVT_PAINT, &FootballCanvas::OnPaintEvent, this);
  this->Bind(wxEVT_CLOSE_WINDOW, &FootballCanvas::OnClose, this);

  this->timer = new CanvasRenderTimer(this);
  this->timer->Start(1000 / fps);

  this->playSound = true;
  this->projectors = true;
  this->soundThread = new AudioThread();
  this->soundThread->Run();
}

FootballStatus FootballCanvas::getStatus() { return this->status; }

void FootballCanvas::enableSounds(bool snd) { this->playSound = snd; }

void FootballCanvas::enableProjectors(bool pr) { this->projectors = pr; }

void FootballCanvas::loadSounds(std::string goalSnd, std::string startSnd) {
  this->goalSound.Create(goalSnd);
  this->startSound.Create(startSnd);
}

void FootballCanvas::setStatus(FootballStatus st) {
  this->status = st;
  wxCommandEvent evt(FOOTBALL_STATUS_CHANGE);
  wxPostEvent(this, evt);
}

void FootballCanvas::OnClose(wxCloseEvent &evt) {
  this->soundThread->exit();
  this->soundThread->Wait();
  this->timer->Stop();
  delete this->timer;
}

void FootballCanvas::OnPaintEvent(wxPaintEvent &evt) {
  wxBufferedPaintDC dc(this);
  this->render(dc);
}

void FootballCanvas::render(wxBufferedPaintDC &dc) {
  float millis = 0.0f;
  if (this->lastClock != 0) {
    millis =
        static_cast<float>(clock() - this->lastClock) / (CLOCKS_PER_SEC / 1000);
    millis = fmod(millis, 1000.0f);
  }
  this->lastClock = clock();

  dc.Clear();

  switch (this->status) {
  case FootballStatus::Playing:
    if (wxGetApp().getGame().process(millis)) {
      wxCommandEvent evt(FOOTBALL_SCORE_CHANGE);
      wxPostEvent(this, evt);
      if (this->goalSound.IsOk() && this->playSound) {
        this->soundThread->play(this->goalSound);
      }
      if (this->projectors) {
        this->setStatus(FootballStatus::Projector);
        this->projector.start();
      } else {
        this->setStatus(FootballStatus::Playing);
        wxCommandEvent evt(FOOTBALL_ROUND);
        wxPostEvent(this, evt);
      }
    }
    this->draw(dc);
    break;
  case FootballStatus::Paused:
    this->draw(dc);
    break;
  case FootballStatus::Projector: {
    wxSize canvasSize = this->GetSize();
    this->draw(dc);
    this->projector.render(dc, canvasSize, millis);
    if (this->projector.finished()) {
      if (this->playSound) {
        this->soundThread->play(this->startSound);
        this->setStatus(FootballStatus::StartWhistle);
      } else {
        this->setStatus(FootballStatus::Playing);
        wxCommandEvent evt(FOOTBALL_ROUND);
        wxPostEvent(this, evt);
      }
    }
  } break;
  case FootballStatus::StartWhistle:
    this->draw(dc);
    if (!this->soundThread->isPlaying()) {
      this->setStatus(FootballStatus::Playing);
      wxCommandEvent evt(FOOTBALL_ROUND);
      wxPostEvent(this, evt);
    }
    break;
  }
  if (millis > 0.0f) {
    this->updateFPS(millis);
  }
}

void FootballCanvas::draw(wxDC &dc) {
  wxSize canvasSize = this->GetSize();
  wxSize backgroundSize = this->background.GetSize();
  float scaleX = static_cast<float>(canvasSize.GetX()) / backgroundSize.GetX();
  float scaleY = static_cast<float>(canvasSize.GetY()) / backgroundSize.GetY();
  dc.SetUserScale(scaleX, scaleY);
  dc.DrawBitmap(this->background, 0, 0);
  dc.SetUserScale(1, 1);

  CanvasGameRender render(dc, canvasSize);
  wxGetApp().getGame().visit(render);

  std::pair<Team *, Team *> teams = wxGetApp().getGame().getTeams();
  Rect &g1 = teams.first->getGoal();
  Rect &g2 = teams.second->getGoal();
  dc.DrawRectangle(canvasSize.GetWidth() * g1.x, canvasSize.GetHeight() * g1.y,
                   canvasSize.GetWidth() * g1.w, canvasSize.GetHeight() * g1.h);
  dc.DrawRectangle(canvasSize.GetWidth() * g2.x, canvasSize.GetHeight() * g2.y,
                   canvasSize.GetWidth() * g2.w, canvasSize.GetHeight() * g2.h);
}

float FootballCanvas::getFPS() {
  return 1000.0f / this->fpsSum * this->fpsCount;
}

void FootballCanvas::updateFPS(float millis) {
  if (this->fpsCount > 100) {
    this->fpsSum -= this->fpsSum / (this->fpsCount--);
  }
  this->fpsSum += millis;
  this->fpsCount++;
}
}