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
};

#endif  // STARTSCENE_HPP__