#ifndef FOOTBALL_FOOTBALL_APP_H_
#define FOOTBALL_FOOTBALL_APP_H_

#include "football/Game.h"
#include "football/ResourceManager.h"
#include "football/base.h"
#include <memory>

namespace Football {

class Team;          // Forward referencing
class FootballFrame; // Forward referencing

class FootballApp : public wxApp {
public:
  bool OnInit() override;
  int OnExit() override;

  ResourceManager &getResourceManager();
  Game &getGame();
  FootballFrame *getFrame();

private:
  std::unique_ptr<ResourceManager> resourceManager;
  Game game;
  FootballFrame *frame;
};
}

wxDECLARE_APP(Football::FootballApp);

#endif