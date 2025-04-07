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
  void keyboardControl(float deltaTime);
  float getRandomNum();
  SDL_FPoint getDirectionVec(const Enemy &enemy);
  void playerShoot();
  void enemyShoot(Enemy &enemy);

 private:
  bool isDead = false;
  std::mt19937 gen_;
  std::uniform_real_distribution<float> dis_;

  Player player_;

  Enemy enemy_tmp_;
  PlayerBullet player_bullet_tmp_;
  EnemyBullet enemy_bullet_tmp_;

  std::list<PlayerBullet *> player_bullets_;
  std::list<Enemy *> enemies_;
  std::list<EnemyBullet *> enemy_bullets_;
};

#endif  // SCENEBASE_HPP__
