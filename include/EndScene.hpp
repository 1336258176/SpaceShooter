#ifndef ENDSCENE_HPP__
#define ENDSCENE_HPP__

#include "scenebase.hpp"

class EndScene : public SceneBase {
 public:
  EndScene() = default;
  ~EndScene() = default;

  void init() override;
  void update(float deltaTime) override;
  void render() override;
  void handleEvent(const SDL_Event& event) override;
  void quit() override;

 private:
};

#endif  // ENDSCENE_HPP__