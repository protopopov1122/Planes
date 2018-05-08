#include "football/FootballApp.h"
#include "football/FootballFrame.h"
#include <ctime>
#include <iostream>
#include <wx/msgdlg.h>

namespace Football {

bool FootballApp::OnInit() {
  std::string path;
  if (this->argc > 1) {
    path = std::string(this->argv[1]);
  } else {
    wxDirDialog openDirDialog(nullptr, _("Select data directory"));
    if (openDirDialog.ShowModal() != wxID_OK) {
      return false;
    }
    path = openDirDialog.GetPath();
  }

  this->resourceManager = std::make_unique<ResourceManager>(path);
  srand(time(NULL));

  wxBitmap bmap = this->resourceManager->loadBitmap("ball.png");
  wxBitmap bt1 = this->resourceManager->loadBitmap("team1.png");
  wxBitmap bt2 = this->resourceManager->loadBitmap("team2.png");
  wxBitmap referee = this->resourceManager->loadBitmap("referee.png");
  wxSize sz(32, 32);
  Drawable ballDraw(bmap, sz);
  std::unique_ptr<Drawable> team1Draw = std::make_unique<Drawable>(bt1, sz);
  std::unique_ptr<Drawable> team2Draw = std::make_unique<Drawable>(bt2, sz);
  Drawable refereeDraw(referee, sz);
  Drawable::setBaseScreenSize(wxSize(1920, 1080));

  BallControllerFactory balls(
      0.1f, Rect(0.035f * 1.25, 0.05f, 1.0f - 2.5f * 0.035f, 1.0f - 2 * 0.05f));
  this->game.setBall(std::make_unique<Ball>(ballDraw, balls));
  this->game.getBall()->setPosition(Position(0.5f, 0.25f));
  std::unique_ptr<Team> team1 = std::make_unique<Team>(
      this->game, this->game, std::move(team1Draw), Position(1.0f, 0.5f),
      Rect(0.04f, 0.45f, 0.008f, 0.1f), 1);
  team1->getLayout().addActivePlayer(ActivePlayerType::Attacker,
                                     Position(0.7f, 0.5f));
  team1->getLayout().addActivePlayer(ActivePlayerType::Attacker,
                                     Position(0.8f, 0.9f));
  team1->getLayout().addActivePlayer(ActivePlayerType::Defender,
                                     Position(0.1f, 0.5f));
  team1->getLayout().addActivePlayer(ActivePlayerType::Halfdefender,
                                     Position(0.3f, 0.5f));

  std::unique_ptr<Team> team2 = std::make_unique<Team>(
      this->game, this->game, std::move(team2Draw), Position(0.0f, 0.5f),
      Rect(1 - 0.04f - 0.008f, 0.45f, 0.008f, 0.1f), 2);
  team2->getLayout() = team1->getLayout();
  this->game.setTeams(std::move(team1), std::move(team2));
  wxGetApp().getGame().round();

  const float linRefereeOffset = 0.025f;
  LinearRefereeControllerFactory linReferees(this->game, 0.1f,
                                             linRefereeOffset);
  std::unique_ptr<Referee> linRef1 =
      std::make_unique<Referee>(refereeDraw, linReferees);
  linRef1->getPosition().y = linRefereeOffset;
  game.addReferee(std::move(linRef1));
  linReferees.setLine(1 - linRefereeOffset);
  std::unique_ptr<Referee> linRef2 =
      std::make_unique<Referee>(refereeDraw, linReferees);
  linRef2->getPosition().y = 1 - linRefereeOffset;
  game.addReferee(std::move(linRef2));
  MainRefereeControllerFactory mainReferees(this->game, 0.1f, 5000.0f, 0.085f);
  game.addReferee(std::make_unique<Referee>(refereeDraw, mainReferees));

  this->frame = new FootballFrame("map.png");
  this->game.setKickHandler(*this->frame);

  this->frame->getCanvas()->loadSounds(
      this->resourceManager->getFullPath("goal.wav"),
      this->resourceManager->getFullPath("whistle.wav"));

  frame->Show(true);
  return true;
}

int FootballApp::OnExit() { return 0; }

ResourceManager &FootballApp::getResourceManager() {
  return *this->resourceManager;
}

Game &FootballApp::getGame() { return this->game; }

FootballFrame *FootballApp::getFrame() { return this->frame; }
}

wxIMPLEMENT_APP(Football::FootballApp);