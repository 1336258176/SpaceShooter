#ifndef GAME_HPP__
#define GAME_HPP__

#include "renderer.hpp"
#include "pch.hpp"
#include "windows.hpp"

class Game final {
 public:
  enum GameState { Start, Running, GameOver };
  GameState state = Start;

  Game& operator=(const Game&) = delete;
  Game(const Game&) = delete;

  void init();

  void quit();

  static Game& getInstance();

  void run();

  void handleEvent();

  void update();

 private:
  Game(Windows&& window, Renderer&& renderer);

  bool shouldColse = false;
  Windows window_;
  Renderer renderer_;
  static std::unique_ptr<Game> instance_;
};

#endif  // GAME_HPP__