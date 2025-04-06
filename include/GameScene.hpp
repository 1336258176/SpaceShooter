#ifndef SCENEBASE_HPP__
#define SCENEBASE_HPP__

#include "entity.hpp"
#include "pch.hpp"
#include "scenebase.hpp"

class GameScene : public SceneBase {
 public:
  GameScene() = default;
  ~GameScene() override = default;

  void init() override;
  void update(float deltaTime) override;
  void render() override;
  void handleEvent(const SDL_Event& event) override;
  void quit() override;
  void keyboardControl(float deltaTime);

 private:
  bool isDead = false;

  Player player_;
};

#endif  // SCENEBASE_HPP__
