#ifndef SCENEBASE_HPP__
#define SCENEBASE_HPP__

#include "entity.hpp"
#include "pch.hpp"
#include "scenebase.hpp"

class GameScene : public SceneBase {
 public:
  GameScene() = default;
  ~GameScene() override;

  void init() override;
  void update(float deltaTime) override;
  void render() override;
  void handleEvent(const SDL_Event &event) override;
  void quit() override;

 private:
  float getRandomNum();
  SDL_FPoint getDirectionVec(const Enemy &enemy);
  void generateItem(const Object& object);

  void playerShoot();
  void enemyShoot(Enemy &enemy);
  void updatePlayer(float deltaTime);
  void keyboardControl(float deltaTime);
  void updataPlayerBullets(float deltaTime);
  void updateEnemies(float deltaTime);
  void updateEnemyBullets(float deltaTime);
  void updateItem(float deltaTime);
  void spawnEnemy();
  void addExplosion(const Object &obj);
  void updateExplosions();

 private:
  int score_ = 0;
  bool isDead_ = false;
  std::mt19937 gen_;
  std::uniform_real_distribution<float> dis_;

  std::unique_ptr<TTF_Font, TTFDeleter> ttf_font_{nullptr, TTFDeleter{}};

  Player player_;
  Enemy enemy_tmp_;
  PlayerBullet player_bullet_tmp_;
  EnemyBullet enemy_bullet_tmp_;
  Explosion explosion_tmp_;
  Item life_item_tmp_;
  HPUI ui_tmp_;

  std::list<PlayerBullet> player_bullets_;
  std::list<Enemy> enemies_;
  std::list<EnemyBullet> enemy_bullets_;
  std::list<Explosion> explosions_;
  std::list<Item> items_;
};

#endif  // SCENEBASE_HPP__
