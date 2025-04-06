#include "GameScene.hpp"

#include "game.hpp"

void GameScene::init() {
  player_.texture = IMG_LoadTexture(game.getRenderer(), "assets/image/SpaceShip.png");
  if (!player_.texture) {
    SDL_LogError(SDL_LOG_CATEGORY_ERROR, "SDL_image load player texture error. %s", SDL_GetError());
  }
  SDL_QueryTexture(player_.texture, NULL, NULL, &player_.width, &player_.height);
  player_.width /= 4;
  player_.height /= 4;
  player_.pos.x = static_cast<float>(game.getWindowWidth() / 2.0 - player_.width / 2.0);
  player_.pos.y = static_cast<float>(game.getWindowHeight() - player_.height);
}

void GameScene::update(float deltaTime) { keyboardControl(deltaTime); }

void GameScene::render() {
  game.renderer_.renderTexture(player_.texture, player_.pos, player_.width, player_.height);
}

void GameScene::handleEvent(const SDL_Event& event) {}

void GameScene::quit() {}

void GameScene::keyboardControl(float deltaTime) {
  auto keyboard = SDL_GetKeyboardState(NULL);
  if (keyboard[SDL_SCANCODE_W]) {
    player_.pos.y -= deltaTime * player_.speed;
    if (player_.pos.y < 0) player_.pos.y = 0;
  }
  if (keyboard[SDL_SCANCODE_S]) {
    player_.pos.y += deltaTime * player_.speed;
    if (player_.pos.y > game.getWindowHeight() - player_.height)
      player_.pos.y = static_cast<float>(game.getWindowHeight() - player_.height);
  }
  if (keyboard[SDL_SCANCODE_A]) {
    player_.pos.x -= deltaTime * player_.speed;
    if (player_.pos.x < 0) player_.pos.x = 0;
  }
  if (keyboard[SDL_SCANCODE_D]) {
    player_.pos.x += deltaTime * player_.speed;
    if (player_.pos.x > game.getWindowWidth() - player_.width)
      player_.pos.x = static_cast<float>(game.getWindowWidth() - player_.width);
  }
}
