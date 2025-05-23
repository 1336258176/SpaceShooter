#include "EndScene.hpp"

#include "game.hpp"

void EndScene::init() {
  game.logger_->info("end-scene init");
  if (!SDL_IsTextInputActive()) {
    SDL_StartTextInput();
    if (!SDL_IsTextInputActive()) {
      game.logger_->error("EndScene: start text input error");
    }
  }

  loadData();
}

void EndScene::update(float deltaTime) {}

void EndScene::render() {
  if (isTyping_) {
    renderGameOver();
  } else {
    renderScoreList();
  }
}

void EndScene::renderGameOver() {
  std::string yourScoreText = "Your Score: " + std::to_string(game.getScore());
  std::string gameoverText = "Game Over";
  std::string inputText = "Input your name(Press enter to end):";

  SDL_FPoint score_pos = {game.getWindowWidth() / 2.8f, game.getWindowHeight() / 6.f};
  game.renderer_
      .renderText(game.getTextFont(), yourScoreText, {255, 255, 255, 255}, score_pos, true, false);
  SDL_FPoint gameover_pos = {game.getWindowWidth() / 5.f, game.getWindowHeight() / 3.f};
  game.renderer_.renderText(game.getTitleFont(),
                            gameoverText,
                            {255, 255, 255, 255},
                            gameover_pos,
                            true,
                            false);
  SDL_FPoint input_pos = {game.getWindowWidth() / 4.f, game.getWindowHeight() / 3.f * 1.7f};
  game.renderer_
      .renderText(game.getTextFont(), inputText, {255, 255, 255, 255}, input_pos, true, false);

  SDL_FPoint underline_pos = {0.f, input_pos.y + 50.f};
  game.renderer_
      .renderText(game.getTextFont(), underline_, {255, 255, 255, 255}, underline_pos, true, false);
  if (name_ != "") {
    SDL_FPoint name_pos = {0.f, underline_pos.y};
    game.renderer_
        .renderText(game.getTextFont(), name_, {255, 255, 255, 255}, name_pos, true, false);
  }
}

void EndScene::saveData() {
  std::ofstream of("assets/save.dat");
  if (!of.is_open()) {
    game.logger_->error("EndScene: save data error");
  }
  for (const auto [name, score] : scores_) {
    of << name << " " << score << std::endl;
  }
}

void EndScene::loadData() {
  if (!std::filesystem::exists("assets/save.dat")) {
    game.logger_->warn("The save does not exist");
    return;
  }
  std::ifstream data("assets/save.dat");
  if (!data.is_open()) {
    game.logger_->error("EndScene: load data error");
  }
  scores_.clear();
  int score;
  std::string name;
  while (data >> name >> score) {
    if (scores_.count(name)) {
      if (score > scores_[name]) scores_[name] = score;
    } else
      scores_[name] = score;
  }
}

void EndScene::renderScoreList() {
  std::string title = "Score List";
  SDL_FPoint title_pos = {0.f, game.getWindowHeight() / 8.f};
  game.renderer_
      .renderText(game.getTitleFont(), title, {255, 255, 255, 255}, title_pos, true, false);

  SDL_FPoint text_base = {0.f, title_pos.y + 50.f};
  float offset_y = 40.f;
  int count = 0;
  char text[256];
  const char* fmt = "%d.%6s                   %4d";
  for (const auto& player : sorted_scores_) {
    count++;
    snprintf(text, sizeof(text), fmt, count, player.first.c_str(), player.second);
    SDL_FPoint text_pos = {0.f, text_base.y + count * offset_y};
    game.renderer_
        .renderText(game.getTextFont(), text, {255, 255, 255, 255}, text_pos, true, false);
    if (count >= 8) break;
  }

  std::string restart_text = "Press Enter to restart the game.";
  SDL_FPoint restart_text_pos = {0.f, title_pos.y + 500.f};
  game.renderer_.renderText(game.getTextFont(),
                            restart_text,
                            {255, 255, 255, 255},
                            restart_text_pos,
                            true,
                            false);
}

void EndScene::handleEvent(const SDL_Event& event) {
  if (event.type == SDL_KEYDOWN) {
    if (event.key.keysym.scancode == SDL_SCANCODE_ESCAPE) {
      game.close();
    }
  }
  if (isTyping_) {
    if (event.type == SDL_TEXTINPUT) {
      if (name_.length() < underline_.length()) name_ += event.text.text;
    }
    if (event.type == SDL_KEYDOWN) {
      switch (event.key.keysym.scancode) {
        case SDL_SCANCODE_RETURN:
          if (!name_.empty()) {
            isTyping_ = false;
            SDL_StopTextInput();
            if (scores_.count(name_)) {
              if (game.getScore() > scores_[name_]) scores_[name_] = game.getScore();
            } else
              scores_[name_] = game.getScore();
            sorted_scores_ =
                std::move(std::vector<std::pair<std::string, int>>(scores_.begin(), scores_.end()));
            std::sort(sorted_scores_.begin(),
                      sorted_scores_.end(),
                      [](const auto& a, const auto& b) {
                        return a.second > b.second;
                      });
          }
          break;

        case SDL_SCANCODE_BACKSPACE:
          if (!name_.empty()) name_.pop_back();
          break;

        default:
          break;
      }
    }
  } else {
    if (event.type == SDL_KEYDOWN && event.key.keysym.scancode == SDL_SCANCODE_RETURN) {
      game.changeScene(Game::GameState::Running);
    }
  }
}

void EndScene::quit() {
  game.logger_->info("save data");
  saveData();
  game.logger_->info("end-scene quit");
}
