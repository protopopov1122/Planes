#ifndef FOOTBALL_TEAM_H_
#define FOOTBALL_TEAM_H_

#include "football/Player.h"
#include "football/base.h"
#include <vector>

namespace Football {

struct TeamPlayer {
  TeamPlayer(ActivePlayerType type, Position pos) : type(type), position(pos) {}
  TeamPlayer(const TeamPlayer &pl) : type(pl.type), position(pl.position) {}
  TeamPlayer &operator=(const TeamPlayer &pl) {
    this->type = pl.type;
    this->position = pl.position;
    return *this;
  }

  ActivePlayerType type;
  Position position;
};

class TeamLayout {
public:
  TeamLayout();

  bool isGoalkeeperEnabled();
  std::vector<TeamPlayer> &getActivePlayers();
  void addActivePlayer(ActivePlayerType, Position pos);
  void enableGoalkeeper(bool);

private:
  std::vector<TeamPlayer> activePlayers;
  bool goalkeeper;
};

class TeamCollection; // Forward referencing

class TeamParameters {
public:
  TeamParameters();

  uint16_t getScore();
  void incScore();
  void resetScore();

  float getEnergy();
  void setEnergy(float);

private:
  uint16_t score;
  float energy;
};

class Team {
public:
  Team(TeamCollection &, KickRegister &, std::unique_ptr<Drawable>, Position,
       Rect, size_t);

  TeamLayout &getLayout();
  TeamParameters &getParameters();
  void reset();
  void spawn(bool = false);
  size_t getID();

  GameObject *getNearestPlayer(Position &);
  void visit(GameVisitor &, bool = true);
  void visit(GameVisitor &, Position &, float);
  bool playerCollides(ControllableObject &, Position &, wxSize &);
  Player &getRandomPlayer();

  Rect &getGoal();
  Position &getKickDestination();

private:
  TeamCollection &teamCollection;
  KickRegister &kickRegister;
  size_t id;
  std::unique_ptr<Drawable> playerDrawable;
  TeamLayout layout;
  TeamParameters parameters;
  Position destination;
  Rect goal;
  std::unique_ptr<AttackerControllerFactory> attackers;
  std::unique_ptr<DefenderControllerFactory> defenders;
  std::unique_ptr<HalfdefenderControllerFactory> halfdefenders;
  std::unique_ptr<GoalkeeperControllerFactory> goalkeepers;
  std::vector<std::unique_ptr<Player>> players;
};

class TeamCollection : public GameObjectCollection {
public:
  virtual ~TeamCollection() = default;
  virtual std::pair<Team *, Team *> getTeams() = 0;
};

class KickRegister {
public:
  virtual ~KickRegister() = default;
  virtual void registerKick(Team &, Player &) = 0;
};
}

#endif