#include "football/FootballFrame.h"
#include "football/FootballApp.h"
#include <wx/sizer.h>

namespace Football {
class UpdateTimer : public wxTimer {
public:
  UpdateTimer(FootballFrame *frame) : frame(frame) {}
  void Notify() override { frame->updateTiming(); }

private:
  FootballFrame *frame;
};

FootballFrame::FootballFrame(std::string background)
    : wxFrame::wxFrame(nullptr, wxID_DEFAULT, "Football", wxDefaultPosition,
                       wxSize(800, 600)) {
  wxPanel *mainPanel = new wxPanel(this);
  wxBoxSizer *sizer = new wxBoxSizer(wxHORIZONTAL);
  mainPanel->SetSizer(sizer);

  this->CreateStatusBar(1);
  this->SetStatusText("");

  this->canvas = new FootballCanvas(mainPanel, wxID_ANY, 100, background);
  sizer->Add(canvas, 1, wxEXPAND);

  this->controlPanel = new wxPanel(mainPanel);
  sizer->Add(controlPanel, 0, wxALL | wxEXPAND, 5);
  wxBoxSizer *controlSizer = new wxBoxSizer(wxVERTICAL);
  controlPanel->SetSizer(controlSizer);

  this->pauseButton = new wxButton(controlPanel, wxID_ANY, "");
  controlSizer->Add(this->pauseButton, 0, wxEXPAND);
  this->pauseButton->Bind(wxEVT_BUTTON, &FootballFrame::ChangeStatus, this);
  this->pauseButton->Enable(false);

  wxButton *resetButton = new wxButton(controlPanel, wxID_ANY, "Reset");
  controlSizer->Add(resetButton, 0, wxEXPAND);
  resetButton->Bind(wxEVT_BUTTON, [&](wxCommandEvent &evt) {
    wxGetApp().getGame().getGoals().clear();
    wxGetApp().getGame().round();
    game_time = 0;
    start_time = clock();
    canvas->setStatus(FootballStatus::Paused);
    team1Panel->update();
    team2Panel->update();
    std::pair<Team *, Team *> teams = wxGetApp().getGame().getTeams();
    teams.first->getParameters().resetScore();
    teams.second->getParameters().resetScore();
    updateScore();
    SetStatusText("");
    goals->SetValue("");
  });

  this->score = new wxStaticText(controlPanel, wxID_ANY, "");
  controlSizer->Add(this->score, 0, wxALIGN_CENTER);

  wxCheckBox *soundCheckbox =
      new wxCheckBox(controlPanel, wxID_ANY, "Enable sound");
  controlSizer->Add(soundCheckbox, 0);
  soundCheckbox->SetValue(true);
  soundCheckbox->Bind(wxEVT_CHECKBOX, [=](wxCommandEvent &evt) {
    canvas->enableSounds(soundCheckbox->GetValue());
  });

  wxCheckBox *projectorsCheckbox =
      new wxCheckBox(controlPanel, wxID_ANY, "Enable projectors");
  controlSizer->Add(projectorsCheckbox, 0);
  projectorsCheckbox->SetValue(true);
  projectorsCheckbox->Bind(wxEVT_CHECKBOX, [=](wxCommandEvent &evt) {
    canvas->enableProjectors(projectorsCheckbox->GetValue());
  });

  std::pair<Team *, Team *> teams = wxGetApp().getGame().getTeams();
  this->team1Panel = new TeamPanel(controlPanel, wxID_ANY, *teams.first);
  controlSizer->Add(new wxStaticText(controlPanel, wxID_ANY, "Team 1:"));
  controlSizer->Add(this->team1Panel);
  this->team2Panel = new TeamPanel(controlPanel, wxID_ANY, *teams.second);
  controlSizer->Add(new wxStaticText(controlPanel, wxID_ANY, "Team 2:"));
  controlSizer->Add(this->team2Panel);

  this->speedSlider = new wxSlider(
      controlPanel, wxID_ANY, 0, -5, 5, wxDefaultPosition, wxDefaultSize,
      wxSL_HORIZONTAL | wxSL_LABELS | wxSL_AUTOTICKS);
  controlSizer->Add(this->speedSlider);
  this->speedSlider->Bind(wxEVT_SLIDER, &FootballFrame::OnSpeedChange, this);

  this->timingText = new wxStaticText(controlPanel, wxID_ANY, "");
  controlSizer->Add(this->timingText);
  this->fpsText = new wxStaticText(controlPanel, wxID_ANY, "");
  controlSizer->Add(this->fpsText);
  controlSizer->Add(new wxStaticText(controlPanel, wxID_ANY, "Goals:"));
  this->goals = new wxTextCtrl(controlPanel, wxID_ANY, "", wxDefaultPosition,
                               wxDefaultSize, wxTE_READONLY | wxTE_MULTILINE);
  controlSizer->Add(this->goals, 1, wxEXPAND);

  this->canvas->Bind(FOOTBALL_STATUS_CHANGE, &FootballFrame::OnStatusChange,
                     this);
  this->canvas->Bind(FOOTBALL_SCORE_CHANGE, &FootballFrame::OnScoreChange,
                     this);
  this->canvas->Bind(FOOTBALL_ROUND, &FootballFrame::OnRound, this);
  this->Bind(wxEVT_CLOSE_WINDOW, &FootballFrame::OnClose, this);
  Maximize(true);

  this->team1Panel->update();
  this->team2Panel->update();

  this->canvas->setStatus(FootballStatus::Projector);
  this->updateScore();

  this->start_time = clock();
  this->game_time = 0;
  this->updateTimer = new UpdateTimer(this);
  this->updateTimer->Start(500);
}

FootballCanvas *FootballFrame::getCanvas() { return this->canvas; }

void FootballFrame::updateScore() {
  std::pair<uint16_t, uint16_t> score = wxGetApp().getGame().getScore();
  this->score->SetLabel(std::to_string(score.first) + "x" +
                        std::to_string(score.second));
}

void FootballFrame::OnClose(wxCloseEvent &evt) {
  this->updateTimer->Stop();
  delete this->updateTimer;
  this->canvas->Close();
  this->Destroy();
}

void FootballFrame::ChangeStatus(wxCommandEvent &evt) {
  this->canvas->setStatus(this->canvas->getStatus() == FootballStatus::Paused
                              ? FootballStatus::Playing
                              : FootballStatus::Paused);
}

void FootballFrame::OnStatusChange(wxCommandEvent &evt) {
  if (this->canvas->getStatus() == FootballStatus::Projector ||
      this->canvas->getStatus() == FootballStatus::StartWhistle) {
    this->pauseButton->Enable(false);
  } else {
    this->pauseButton->SetLabel(
        this->canvas->getStatus() == FootballStatus::Paused ? "Run" : "Pause");
    this->pauseButton->Enable(true);
  }
  if (this->canvas->getStatus() == FootballStatus::Playing) {
    this->start_time = clock();
  } else {
    if (this->start_time > 0) {
      this->game_time += clock() - this->start_time;
    }
    this->start_time = 0;
  }
}

void FootballFrame::OnScoreChange(wxCommandEvent &evt) {
  this->updateScore();
  this->SetStatusText("");
  this->goals->SetValue("");
  for (auto &goal : wxGetApp().getGame().getGoals()) {
    *this->goals << wxString("Team " + std::to_string(goal.first) + " Player " +
                             std::to_string(goal.second) + "\n");
  }
}

void FootballFrame::OnSpeedChange(wxCommandEvent &evt) {
  int value = this->speedSlider->GetValue();
  wxGetApp().getGame().setSpeed(value >= 0 ? 1 + value : 1.0f / (-value + 1));
}

void FootballFrame::OnRound(wxCommandEvent &evt) {
  wxGetApp().getGame().round();
  this->team1Panel->update();
  this->team2Panel->update();
}

void FootballFrame::updateTiming() {
  if (this->canvas->getStatus() == FootballStatus::Playing) {
    int raw_speed = this->speedSlider->GetValue();
    float speed = raw_speed >= 0 ? 1 + raw_speed : 1.0f / (-raw_speed + 1);
    clock_t delta = static_cast<clock_t>((clock() - this->start_time) * speed);
    this->game_time += delta;
    this->start_time = clock();
  }
  clock_t time_from_start = this->game_time / (CLOCKS_PER_SEC / 1000) / 1000;
  this->timingText->SetLabel("Time: " + std::to_string(time_from_start) + " s");
  this->fpsText->SetLabel(
      "FPS: " + std::to_string(static_cast<int>(this->canvas->getFPS())));
}

void FootballFrame::registerKick() {
  std::pair<Team *, Player *> kick = wxGetApp().getGame().getLastKick();
  if (kick.first != nullptr && kick.second != nullptr) {
    this->SetStatusText("Team " + std::to_string(kick.first->getID()) +
                        " Player " + std::to_string(kick.second->getID()));
  }
}
}