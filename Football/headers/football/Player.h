#ifndef FOOTBALL_PLAYER_H_
#define FOOTBALL_PLAYER_H_

#include "football/Behavior.h"

namespace Football {

class Team;           // Forward referencing
class TeamCollection; // Forward referencing
class KickRegister;   // Forward referencing

enum class ActivePlayerType { Attacker, Defender, Halfdefender };

class KickHandler {
public:
  virtual ~KickHandler() = default;
  virtual void registerKick() = 0;
};

class PlayerControllerFactory {
public:
  PlayerControllerFactory(TeamCollection &, Team &, float, float);
  virtual std::unique_ptr<ObjectController> create(ControllableObject &,
                                                   KickHandler *) = 0;

protected:
  TeamCollection &gameObjects;
  Team &team;
  float speed;
  float kickStrength;
};

class PlayerController : public BehaviorController {
public:
  PlayerController(ControllableObject &, TeamCollection &, Team &, float,
                   float);
  Team &getTeam();
  void setKickHandler(KickHandler *);

protected:
  void kick(PassiveObject &, Position &, float);
  void smartKick(PassiveObject &, Position &, float);
  float kickCorrection(float);

  TeamCollection &teams;
  Team &team;
  KickHandler *kickHandler;
  float kickStrength;
};

class Player : public GameObject, public KickHandler {
public:
  Player(Drawable &, Team &, KickRegister &, PlayerControllerFactory &, size_t);
  PlayerController &getBehavior();
  void registerKick() override;

  Team &getTeam();
  bool isEnabled() const;
  void enable(bool);
  size_t getID();

private:
  Team &team;
  KickRegister &kickRegister;
  bool enabled;
  size_t id;
};

class PlayerVisitor : public GameVisitor {
public:
  void visit(GameObject &obj) {
    Player *player = dynamic_cast<Player *>(&obj);
    if (player) {
      this->visit(*player);
    }
  }
  virtual void visit(Player &) = 0;
};

class AttackerController : public PlayerController {
public:
  AttackerController(ControllableObject &, TeamCollection &, Team &, float,
                     float);

protected:
  virtual void play(PassiveObject &, float);
};

class AttackerControllerFactory : public PlayerControllerFactory {
public:
  AttackerControllerFactory(TeamCollection &, Team &, float, float);
  std::unique_ptr<ObjectController> create(ControllableObject &,
                                           KickHandler *) override;
};

class DefenderController : public PlayerController {
public:
  DefenderController(ControllableObject &, TeamCollection &, Team &, float,
                     float);

protected:
  virtual void play(PassiveObject &, float);
};

class DefenderControllerFactory : public PlayerControllerFactory {
public:
  DefenderControllerFactory(TeamCollection &, Team &, float, float);
  std::unique_ptr<ObjectController> create(ControllableObject &,
                                           KickHandler *) override;
};

class HalfdefenderController : public PlayerController {
public:
  HalfdefenderController(ControllableObject &, TeamCollection &, Team &, float,
                         float, float);

protected:
  virtual void play(PassiveObject &, float);

private:
  float range;
};

class HalfdefenderControllerFactory : public PlayerControllerFactory {
public:
  HalfdefenderControllerFactory(TeamCollection &, Team &, float, float, float);
  std::unique_ptr<ObjectController> create(ControllableObject &,
                                           KickHandler *) override;

private:
  float range;
};

class GoalkeeperController : public PlayerController {
public:
  GoalkeeperController(ControllableObject &, TeamCollection &, Team &, float,
                       float, float);

protected:
  virtual void play(PassiveObject &, float);

private:
  float range;
};

class GoalkeeperControllerFactory : public PlayerControllerFactory {
public:
  GoalkeeperControllerFactory(TeamCollection &, Team &, float, float, float);
  std::unique_ptr<ObjectController> create(ControllableObject &,
                                           KickHandler *) override;

private:
  float range;
};
}

#endif