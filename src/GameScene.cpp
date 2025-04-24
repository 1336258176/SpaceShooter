#include "GameScene.hpp"

#include "game.hpp"

void GameScene::init() {
  std::random_device rd;
  gen_ = std::mt19937(rd());
  dis_ = std::uniform_real_distribution<float>(0.f, 1.f);

  // player
  player_.setTexture(IMG_LoadTexture(game.getRenderer(), PlayerTexturePath));
  if (!player_.texture) {
    SDL_LogError(SDL_LOG_CATEGORY_ERROR, "SDL_image load player texture error.");
  }
  SDL_QueryTexture(player_.getTexture(), NULL, NULL, &player_.width, &player_.height);
  player_.width /= 4;
  player_.height /= 4;
  player_.pos.x = static_cast<float>(game.getWindowWidth() / 2.0 - player_.width / 2.0);
  player_.pos.y = static_cast<float>(game.getWindowHeight() - player_.height);

  // player bullet
  player_bullet_tmp_.setTexture(IMG_LoadTexture(game.getRenderer(), PlayerBulletTexturePath));
  if (!player_bullet_tmp_.texture) {
    SDL_LogError(SDL_LOG_CATEGORY_ERROR, "SDL_image load player bullet texture error.");
  }
  SDL_QueryTexture(player_bullet_tmp_.getTexture(),
                   NULL,
                   NULL,
                   &player_bullet_tmp_.width,
                   &player_bullet_tmp_.height);
  player_bullet_tmp_.width /= 4;
  player_bullet_tmp_.height /= 4;

  // enemy
  enemy_tmp_.setTexture(IMG_LoadTexture(game.getRenderer(), EnemyTexturePath));
  if (!enemy_tmp_.texture) {
    SDL_LogError(SDL_LOG_CATEGORY_ERROR, "SDL_image load enemy texture error.");
  }
  SDL_QueryTexture(enemy_tmp_.getTexture(), NULL, NULL, &enemy_tmp_.width, &enemy_tmp_.height);
  enemy_tmp_.width /= 4;
  enemy_tmp_.height /= 4;

  // enemy bullet
  enemy_bullet_tmp_.setTexture(IMG_LoadTexture(game.getRenderer(), EnemyBulletTexturePath));
  if (!enemy_bullet_tmp_.texture) {
    SDL_LogError(SDL_LOG_CATEGORY_ERROR, "SDL_image load enemy bullet texture error.");
  }
  SDL_QueryTexture(enemy_bullet_tmp_.getTexture(),
                   NULL,
                   NULL,
                   &enemy_bullet_tmp_.width,
                   &enemy_bullet_tmp_.height);
  enemy_bullet_tmp_.width /= 4;
  enemy_bullet_tmp_.height /= 4;

  // explosion
  explosion_tmp_.setTexture(IMG_LoadTexture(game.getRenderer(), ExplosionTexturePath));
  if (!explosion_tmp_.texture) {
    SDL_LogError(SDL_LOG_CATEGORY_ERROR, "SDL_image load explosion texture error.");
  }
  SDL_QueryTexture(explosion_tmp_.getTexture(),
                   NULL,
                   NULL,
                   &explosion_tmp_.width,
                   &explosion_tmp_.height);
  explosion_tmp_.totalFrame = explosion_tmp_.width / explosion_tmp_.height;
  explosion_tmp_.width = explosion_tmp_.height;

  // item
  life_item_tmp_.setTexture(IMG_LoadTexture(game.getRenderer(), LifeItemTexturePath));
  if (!life_item_tmp_.texture) {
    SDL_LogError(SDL_LOG_CATEGORY_ERROR, "SDL_image load LifeItem texture error.");
  }
  SDL_QueryTexture(life_item_tmp_.getTexture(),
                   NULL,
                   NULL,
                   &life_item_tmp_.width,
                   &life_item_tmp_.height);
  life_item_tmp_.width /= 4;
  life_item_tmp_.height /= 4;

  // UI
  ui_tmp_.setTexture(IMG_LoadTexture(game.getRenderer(), HPUITexturePath));
  if (!ui_tmp_.texture) {
    SDL_LogError(SDL_LOG_CATEGORY_ERROR, "SDL_image load HP-UI texture error.");
  }
  SDL_QueryTexture(ui_tmp_.getTexture(), NULL, NULL, &ui_tmp_.width, &ui_tmp_.height);
  ui_tmp_.offset = static_cast<float>(ui_tmp_.width);
}

void GameScene::update(float deltaTime) {
  updatePlayer(deltaTime);
  updataPlayerBullets(deltaTime);
  updateItem(deltaTime);

  spawnEnemy();
  updateEnemies(deltaTime);
  updateEnemyBullets(deltaTime);

  updateExplosions();
}

void GameScene::render() {
  // items
  for (const auto& item : items_) {
    game.renderer_.renderTexture(item.getTexture(), item.pos, item.width, item.height);
  }

  // player bullets
  for (const auto& bullet : player_bullets_) {
    game.renderer_.renderTexture(bullet.getTexture(), bullet.pos, bullet.width, bullet.height);
  }

  // enemy bullets
  for (const auto& bullet : enemy_bullets_) {
    double angle = atan2(bullet.direction_vec.y, bullet.direction_vec.x) * 180.0 / M_PI - 90.0;
    SDL_Rect dst = {static_cast<int>(bullet.pos.x),
                    static_cast<int>(bullet.pos.y),
                    bullet.width,
                    bullet.height};
    game.renderer_.renderTextureEx(bullet.getTexture(), dst, angle);
  }

  // player
  if (!isDead) {
    game.renderer_.renderTexture(player_.getTexture(), player_.pos, player_.width, player_.height);
  }

  // enemies
  for (const auto& enemy : enemies_) {
    game.renderer_.renderTexture(enemy.getTexture(), enemy.pos, enemy.width, enemy.height);
  }

  // explosion
  for (auto& explosion : explosions_) {
    SDL_Rect src = {explosion.currentFrame * explosion.width, 0, explosion.width, explosion.height};
    game.renderer_.renderTexture(explosion.getTexture(),
                                 explosion.pos,
                                 explosion.width,
                                 explosion.height,
                                 &src);
  }

  // UI
  for (int i = 0; i < player_.health; i++) {
    SDL_FPoint dst_pos = {i * ui_tmp_.offset + ui_tmp_.pos.x, ui_tmp_.pos.y};
    game.renderer_.renderTexture(ui_tmp_.getTexture(), dst_pos, ui_tmp_.width, ui_tmp_.height);
  }
  SDL_SetTextureColorMod(ui_tmp_.getTexture(), 100, 100, 100);
  for (int i = player_.health; i < PlayerMaxHP; i++) {
    SDL_FPoint dst_pos = {i * ui_tmp_.offset + ui_tmp_.pos.x, ui_tmp_.pos.y};
    game.renderer_.renderTexture(ui_tmp_.getTexture(), dst_pos, ui_tmp_.width, ui_tmp_.height);
  }
  SDL_SetTextureColorMod(ui_tmp_.getTexture(), 255, 255, 255);
}

GameScene::~GameScene() { quit(); }

void GameScene::handleEvent(const SDL_Event& event) {}

void GameScene::quit() {}

void GameScene::keyboardControl(float deltaTime) {
  if (isDead) return;
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
    playerShoot();
  }
}

/**
 * @brief 返回0~1之间的浮点数
 *
 * @return float
 */
float GameScene::getRandomNum() { return dis_(gen_); }

SDL_FPoint GameScene::getDirectionVec(const Enemy& enemy) {
  SDL_FPoint enemy_center = {enemy.pos.x + enemy.width / 2.0f, enemy.pos.y + enemy.height / 2.0f};
  SDL_FPoint player_center = {player_.pos.x + player_.width / 2.0f,
                              player_.pos.y + player_.height / 2.0f};
  SDL_FPoint v = {player_center.x - enemy_center.x, player_center.y - enemy_center.y};
  float l = SDL_sqrtf(v.x * v.x + v.y * v.y);
  return l == 0 ? SDL_FPoint{0.f, 0.f} : SDL_FPoint{v.x / l, v.y / l};
}

void GameScene::generateItem(const Object& object) {
  if (getRandomNum() > GenerateLifeItemProbability) return;
  Item tmp(life_item_tmp_);
  tmp.pos.x = object.pos.x + (object.width - tmp.width) / 2.0f;
  tmp.pos.y = object.pos.y + (object.height - tmp.height) / 2.0f;
  float angle = static_cast<float>(getRandomNum() * 2 * M_PI);
  tmp.direction_vec.x = std::cos(angle);
  tmp.direction_vec.y = std::sin(angle);
  items_.emplace_back(tmp);
}

void GameScene::playerShoot() {
  auto currentTime = SDL_GetTicks();
  if (currentTime - player_.last_shoot_stamp > player_.bullet_cooldown) {
    PlayerBullet tmp(player_bullet_tmp_);
    tmp.pos.x = player_.pos.x + player_.width / 2.0f - player_bullet_tmp_.width / 2.0f;
    tmp.pos.y = player_.pos.y;
    player_bullets_.emplace_back(tmp);
    player_.last_shoot_stamp = currentTime;
  }
}

void GameScene::enemyShoot(Enemy& enemy) {
  auto currentTime = SDL_GetTicks();
  if (currentTime - enemy.last_shoot_stamp > enemy.bullet_cooldown) {
    EnemyBullet tmp(enemy_bullet_tmp_);
    tmp.direction_vec = getDirectionVec(enemy);
    tmp.pos.x = enemy.pos.x + enemy.width / 2.0f;
    tmp.pos.y = enemy.pos.y + enemy.height / 2.0f;
    enemy_bullets_.emplace_back(tmp);
    enemy.last_shoot_stamp = currentTime;
  }
}

void GameScene::updatePlayer(float deltaTime) {
  if (player_.health <= 0) {
    isDead = true;
    addExplosion(player_);
  }
  keyboardControl(deltaTime);
  for (auto it = enemies_.begin(); it != enemies_.end();) {
    auto& enemy = *it;
    SDL_FRect player_model = {player_.pos.x,
                              player_.pos.y,
                              static_cast<float>(player_.width),
                              static_cast<float>(player_.height)};
    SDL_FRect enemy_model = {enemy.pos.x,
                             enemy.pos.y,
                             static_cast<float>(enemy.width),
                             static_cast<float>(enemy.height)};
    if (SDL_HasIntersectionF(&player_model, &enemy_model) && !isDead) {
      player_.health--;
      addExplosion(enemy);
      enemy.isDead = true;
      it = enemies_.erase(it);
    } else
      it++;
  }
}

void GameScene::updataPlayerBullets(float deltaTime) {
  for (auto it = player_bullets_.begin(); it != player_bullets_.end();) {
    auto& bullet = *it;
    bullet.pos.y -= deltaTime * bullet.speed;
    if (bullet.pos.y + bullet.height < 0) {
      it = player_bullets_.erase(it);
    } else {
      bool hit = false;
      for (auto& enemy : enemies_) {
        SDL_FRect bullet_model = {bullet.pos.x,
                                  bullet.pos.y,
                                  static_cast<float>(bullet.width),
                                  static_cast<float>(bullet.height)};
        SDL_FRect enemy_model = {enemy.pos.x,
                                 enemy.pos.y,
                                 static_cast<float>(enemy.width),
                                 static_cast<float>(enemy.height)};
        if (SDL_HasIntersectionF(&bullet_model, &enemy_model)) {
          enemy.health -= bullet.damage;
          if (enemy.health <= 0) {
            generateItem(enemy);
            enemy.isDead = true;
          }
          it = player_bullets_.erase(it);
          hit = true;
          break;
        }
      }
      if (!hit) it++;
    }
  }
}

void GameScene::updateEnemies(float deltaTime) {
  for (auto it = enemies_.begin(); it != enemies_.end();) {
    auto& enemy = *it;
    enemy.pos.y += deltaTime * enemy.speed;
    if (enemy.pos.y > game.getWindowHeight() || enemy.isDead) {
      addExplosion(enemy);
      it = enemies_.erase(it);
    } else {
      if (!isDead) enemyShoot(enemy);
      it++;
    }
  }
}

void GameScene::updateEnemyBullets(float deltaTime) {
  for (auto it = enemy_bullets_.begin(); it != enemy_bullets_.end();) {
    auto& bullet = *it;
    bullet.pos.x += deltaTime * bullet.speed * bullet.direction_vec.x;
    bullet.pos.y += deltaTime * bullet.speed * bullet.direction_vec.y;
    if (bullet.pos.x > game.getWindowWidth() || bullet.pos.y > game.getWindowHeight() ||
        bullet.pos.x + bullet.width < 0 || bullet.pos.y + bullet.height < 0) {
      it = enemy_bullets_.erase(it);
    } else {
      SDL_FRect bullet_model = {bullet.pos.x,
                                bullet.pos.y,
                                static_cast<float>(bullet.width),
                                static_cast<float>(bullet.height)};
      SDL_FRect player_model = {player_.pos.x,
                                player_.pos.y,
                                static_cast<float>(player_.width),
                                static_cast<float>(player_.height)};
      if (SDL_HasIntersectionF(&bullet_model, &player_model)) {
        player_.health -= bullet.damage;
        it = enemy_bullets_.erase(it);
      } else
        it++;
    }
  }
}

void GameScene::updateItem(float deltaTime) {
  for (auto it = items_.begin(); it != items_.end();) {
    auto& item = *it;

    item.pos.x += deltaTime * item.speed * item.direction_vec.x;
    item.pos.y += deltaTime * item.speed * item.direction_vec.y;
    if (item.pos.x <= 0 && item.CollisionCount > 0) {
      item.direction_vec.x = -item.direction_vec.x;
      item.CollisionCount--;
    } else if (item.pos.y <= 0 && item.CollisionCount > 0) {
      item.direction_vec.y = -item.direction_vec.y;
      item.CollisionCount--;
    } else if (item.pos.x + item.width >= game.getWindowWidth() && item.CollisionCount > 0) {
      item.direction_vec.x = -item.direction_vec.x;
      item.CollisionCount--;
    } else if (item.pos.y + item.height >= game.getWindowHeight() && item.CollisionCount > 0) {
      item.direction_vec.y = -item.direction_vec.y;
      item.CollisionCount--;
    }
    if (item.CollisionCount == 0) {
      it = items_.erase(it);
      break;
    }
    SDL_Rect item_model = {static_cast<int>(item.pos.x),
                           static_cast<int>(item.pos.y),
                           item.width,
                           item.height};
    SDL_Rect player_model = {static_cast<int>(player_.pos.x),
                             static_cast<int>(player_.pos.y),
                             player_.width,
                             player_.height};
    if (SDL_HasIntersection(&item_model, &player_model)) {
      it = items_.erase(it);
      player_.health = PlayerMaxHP;
    } else {
      it++;
    }
  }
}

void GameScene::spawnEnemy() {
  if (getRandomNum() < 1.f / game.getFPS() && !isDead) {
    Enemy tmp(enemy_tmp_);
    tmp.pos.x = getRandomNum() * (game.getWindowWidth() - tmp.width);
    tmp.pos.y = static_cast<float>(-tmp.height);
    enemies_.emplace_back(tmp);
  }
}

void GameScene::addExplosion(const Object& obj) {
  Explosion explosion(explosion_tmp_);
  explosion.pos.x = obj.pos.x + obj.width / 2.f - explosion.width / 2.f;
  explosion.pos.y = obj.pos.y + obj.height / 2.f - explosion.height / 2.f;
  explosion.startTime = SDL_GetTicks();
  explosions_.emplace_back(explosion);
}

void GameScene::updateExplosions() {
  auto currentTime = SDL_GetTicks();
  for (auto it = explosions_.begin(); it != explosions_.end();) {
    auto& explosion = *it;
    explosion.currentFrame =
        static_cast<int>((currentTime - explosion.startTime) / 1000.f * explosion.FPS);
    if (explosion.currentFrame > explosion.totalFrame) {
      it = explosions_.erase(it);
    } else {
      it++;
    }
  }
}
