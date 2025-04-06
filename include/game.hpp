#ifndef GAME_HPP__
#define GAME_HPP__

#include "EndScene.hpp"
#include "GameScene.hpp"
#include "StartScene.hpp"
#include "pch.hpp"
#include "renderer.hpp"
#include "scenebase.hpp"
#include "windows.hpp"

class Game final {
 public:
  enum GameState { Start, Running, GameOver };
  GameState state;

  Game& operator=(const Game&) = delete;
  Game(const Game&) = delete;

  void init();
  void quit();
  static Game& getInstance();
  void run();
  void handleEvent(SDL_Event& event);
  void update(float deltaTime);
  void present();
  void changeScene(GameState next_game_state);

  SDL_Renderer* getRenderer();
  SDL_Window* getWindow();
  int getWindowHeight() const;
  int getWindowWidth() const;

  Windows window_;
  Renderer renderer_;

 private:
  Game(Windows&& window, Renderer&& renderer);

  bool shouldColse = false;
  const Uint32 FPS = 60;
  const Uint32 frameTime_ = 1000 / FPS;
  float deltaTime_ = 0.f;
  std::unique_ptr<SceneBase> currentScene_;
  static std::unique_ptr<Game> instance_;
};

#endif  // GAME_HPP__