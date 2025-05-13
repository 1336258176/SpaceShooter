#include "StartScene.hpp"

#include "game.hpp"

void StartScene::init() {
  game.logger_->info("start-scene init");
  title_text_pos_ = {game.getWindowWidth() / 12.f, game.getWindowHeight() / 3.5f};
  start_game_text_pos_ = {game.getWindowWidth() / 5.f, game.getWindowHeight() * 2.f / 3.f};
}

void StartScene::update(float deltaTime) {
  timer_ += deltaTime;
  if (timer_ > 1.f) {
    timer_ -= 1.f;
  }
}

void StartScene::render() {
  game.renderer_.renderText(game.getTitleFont(),
                            title_text_,
                            {255, 255, 255, 255},
                            title_text_pos_,
                            true,
                            false);
  if (timer_ > 0.5f) {
    game.renderer_.renderText(game.getTextFont(),
                              start_game_text_,
                              {255, 255, 255, 255},
                              start_game_text_pos_,
                              true,
                              false);
  }
}

void StartScene::handleEvent(const SDL_Event& event) {
  if (event.type == SDL_KEYDOWN) {
    switch (event.key.keysym.scancode) {
      case SDL_SCANCODE_J:
        game.changeScene(Game::GameState::Running);
        break;
      case SDL_SCANCODE_ESCAPE:
        game.close();
        break;
      default:
        break;
    }
  }
}

void StartScene::quit() { game.logger_->info("start-scene quit"); }
