#ifndef SCENE_HPP__
#define SCENE_HPP__

#include "pch.hpp"

class Game;

class SceneBase {
 public:
  SceneBase();
  virtual ~SceneBase() = default;

  virtual void init() = 0;
  virtual void update(float deltaTime) = 0;
  virtual void render() = 0;
  virtual void handleEvent(const SDL_Event& event) = 0;
  virtual void quit() = 0;

 protected:
  Game& game;
};

#endif  // SCENE_HPP__