#ifndef STARTSCENE_HPP__
#define STARTSCENE_HPP__

#include "renderer.hpp"
#include "scenebase.hpp"
#include "windows.hpp"

class StartScene : public SceneBase {
 public:
  StartScene() = default;
  ~StartScene() override = default;

  void init() override;
  void update(float deltaTime) override;
  void render() override;
  void handleEvent(const SDL_Event& event) override;
  void quit() override;

 private:
  float timer_ = 0.f;

  std::string title_text_ = "Space Shooter";
  std::string start_game_text_ = "Press J to start the game...";

  SDL_FPoint title_text_pos_{0.f, 0.f};
  SDL_FPoint start_game_text_pos_{0.f, 0.f};
};

#endif  // STARTSCENE_HPP__