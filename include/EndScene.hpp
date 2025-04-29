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
  void renderScoreList();
  void renderGameOver();

  void saveData();
  void loadData();

  bool isTyping_ = true;
  std::string name_ = "";
  const std::string underline_ = "______";
  std::unordered_map<std::string, int> scores_;
  std::vector<std::pair<std::string, int>> sorted_scores_;
};

#endif  // ENDSCENE_HPP__