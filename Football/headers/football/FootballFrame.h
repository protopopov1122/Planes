#ifndef FOOTBALL_FOOTBALL_FRAME_H_
#define FOOTBALL_FOOTBALL_FRAME_H_

#include "football/FootballCanvas.h"
#include "football/TeamPanel.h"
#include "football/base.h"
#include <vector>

namespace Football {

class FootballFrame : public wxFrame, public KickHandler {
public:
  FootballFrame(std::string);
  FootballCanvas *getCanvas();
  void updateTiming();
  void registerKick() override;

private:
  void updateScore();
  void OnClose(wxCloseEvent &);
  void ChangeStatus(wxCommandEvent &);
  void OnStatusChange(wxCommandEvent &);
  void OnScoreChange(wxCommandEvent &);
  void OnSpeedChange(wxCommandEvent &);
  void OnRound(wxCommandEvent &);

  FootballCanvas *canvas;
  wxTimer *updateTimer;
  wxPanel *controlPanel;
  wxButton *pauseButton;
  wxStaticText *score;
  TeamPanel *team1Panel;
  TeamPanel *team2Panel;
  wxSlider *speedSlider;
  wxStaticText *timingText;
  wxStaticText *fpsText;
  wxTextCtrl *goals;
  clock_t game_time;
  clock_t start_time;
};
}

#endif