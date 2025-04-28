#include "EndScene.hpp"

#include "game.hpp"

void EndScene::init() {
  if (!SDL_IsTextInputActive()) {
    SDL_StartTextInput();
    if (!SDL_IsTextInputActive()) {
      SDL_LogError(SDL_LOG_CATEGORY_ERROR, "EndScene: start text input error");
    }
  }
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

void EndScene::renderScoreList() {
  std::string title = "Score List";
  SDL_FPoint title_pos = {0.f, game.getWindowHeight() / 8.f};
  game.renderer_
      .renderText(game.getTitleFont(), title, {255, 255, 255, 255}, title_pos, true, false);
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

void EndScene::quit() {}
