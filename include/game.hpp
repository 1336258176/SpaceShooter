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
  GameState state_;
  
  Game &operator=(const Game &) = delete;
  Game(const Game &) = delete;
  ~Game();

  void init();
  void quit();
  static Game &getInstance();
  void run();
  void handleEvent(SDL_Event &event);
  void update(float deltaTime);
  void present();
  void changeScene(GameState next_game_state);

  void setScore(int score);

  SDL_Renderer *getRenderer();
  SDL_Window *getWindow();
  int getWindowHeight() const;
  int getWindowWidth() const;
  int getFPS() const;
  TTF_Font* getTitleFont() const;
  TTF_Font* getTextFont() const;
  int getScore() const;
  void close();

  Windows window_;
  Renderer renderer_;

 private:
  Game(Windows &&window, Renderer &&renderer);

  void updateBackground(float deltaTime);
  void renderBackground();

  BackGround nearStar;
  BackGround farStar;

  std::unique_ptr<TTF_Font, TTFDeleter> title_font_{nullptr, TTFDeleter{} };
  std::unique_ptr<TTF_Font, TTFDeleter> text_font_{nullptr, TTFDeleter{} };

  bool shouldColse_ = false;
  int score_ = 0;
  const Uint32 FPS = 60;
  const Uint32 frameTime_ = 1000 / FPS;  // ms
  float deltaTime_ = 0.f;  // The time(s) difference between this frame and the previous frame
  std::unique_ptr<SceneBase> currentScene_;

  static std::unique_ptr<Game> instance_;
};

#endif  // GAME_HPP__