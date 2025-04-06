#include "GameScene.hpp"

#include "game.hpp"

void GameScene::init() {
  std::random_device rd;
  gen_ = std::mt19937(rd());
  dis_ = std::uniform_real_distribution<float>(0.f, 1.f);

  player_.texture = IMG_LoadTexture(game.getRenderer(), "assets/image/SpaceShip.png");
  if (!player_.texture) {
    SDL_LogError(SDL_LOG_CATEGORY_ERROR, "SDL_image load player texture error.");
  }
  SDL_QueryTexture(player_.texture, NULL, NULL, &player_.width, &player_.height);
  player_.width /= 4;
  player_.height /= 4;
  player_.pos.x = static_cast<float>(game.getWindowWidth() / 2.0 - player_.width / 2.0);
  player_.pos.y = static_cast<float>(game.getWindowHeight() - player_.height);

  player_bullet_tmp_.texture = IMG_LoadTexture(game.getRenderer(), "assets/image/laser-1.png");
  if (!player_bullet_tmp_.texture) {
    SDL_LogError(SDL_LOG_CATEGORY_ERROR, "SDL_image load player bullet texture error.");
  }
  SDL_QueryTexture(player_bullet_tmp_.texture, NULL, NULL, &player_bullet_tmp_.width,
                   &player_bullet_tmp_.height);
  player_bullet_tmp_.width /= 4;
  player_bullet_tmp_.height /= 4;

  enemy_tmp_.texture = IMG_LoadTexture(game.getRenderer(), "assets/image/insect-2.png");
  if (!enemy_tmp_.texture) {
    SDL_LogError(SDL_LOG_CATEGORY_ERROR, "SDL_image load enemy texture error.");
  }
  SDL_QueryTexture(enemy_tmp_.texture, NULL, NULL, &enemy_tmp_.width, &enemy_tmp_.height);
  enemy_tmp_.width /= 4;
  enemy_tmp_.height /= 4;
}

void GameScene::update(float deltaTime) {
  keyboardControl(deltaTime);
  for (auto it = player_bullets_.begin(); it != player_bullets_.end();) {
    auto bullet = *it;
    bullet->pos.y -= deltaTime * bullet->speed;
    if (bullet->pos.y + bullet->height < 0) {
      delete bullet;
      it = player_bullets_.erase(it);
    } else {
      it++;
    }
  }

  // spaw enemy: one enemy per second
  if (getRandomNum() < 1.f / game.getFPS()) {
    auto tmp = new Enemy(enemy_tmp_);
    tmp->pos.x = getRandomNum() * (game.getWindowWidth() - tmp->width);
    tmp->pos.y = static_cast<float>(-tmp->height);
    enemies_.push_back(tmp);
  }

  for (auto it = enemies_.begin(); it != enemies_.end();) {
    auto enemy = *it;
    enemy->pos.y += deltaTime * enemy->speed;
    if (enemy->pos.y > game.getWindowHeight()) {
      delete enemy;
      it = enemies_.erase(it);
    } else {
      it++;
    }
  }
}

void GameScene::render() {
  for (const auto& bullet : player_bullets_) {
    game.renderer_.renderTexture(bullet->texture, bullet->pos, bullet->width, bullet->height);
  }
  game.renderer_.renderTexture(player_.texture, player_.pos, player_.width, player_.height);
  for (const auto& enemy : enemies_) {
    game.renderer_.renderTexture(enemy->texture, enemy->pos, enemy->width, enemy->height);
  }
}

GameScene::~GameScene() { quit(); }

void GameScene::handleEvent(const SDL_Event& event) {}

void GameScene::quit() {
  for (auto& bullet : player_bullets_) {
    delete bullet;
  }
  for (auto& enemy : enemies_) {
    delete enemy;
  }
}

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
  if (keyboard[SDL_SCANCODE_J]) {
    auto currentTime = SDL_GetTicks();
    if (currentTime - player_.last_shoot_stamp > player_.bullet_cooldown) {
      auto tmp = new PlayerBullet(player_bullet_tmp_);
      tmp->pos.x = player_.pos.x + player_.width / 2.0f - player_bullet_tmp_.width / 2.0f;
      tmp->pos.y = player_.pos.y;
      player_bullets_.push_back(tmp);
      player_.last_shoot_stamp = currentTime;
    }
  }
}

float GameScene::getRandomNum() { return dis_(gen_); }
