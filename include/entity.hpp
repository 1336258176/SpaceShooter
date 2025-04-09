#ifndef ENTITY_HPP__
#define ENTITY_HPP__

#include "pch.hpp"

constexpr int PlayerHealth = 3;
constexpr int EnemyHealth = 2;
constexpr int PlayerBulletDamage = 1;
constexpr int EnemyBulletDamage = 1;

constexpr int PlayerSpeed = 300;
constexpr int PlayerBulletSpeed = 600;
constexpr int PlayerShootCooldown = 300;
constexpr int EnemySpeed = 200;
constexpr int EnemyBulletSpeed = 400;
constexpr int EnemyShootCooldown = 2000;

inline const char* PlayerTexturePath = "assets/image/SpaceShip.png";
inline const char* PlayerBulletTexturePath = "assets/image/laser-1.png";
inline const char* EnemyTexturePath = "assets/image/insect-2.png";
inline const char* EnemyBulletTexturePath = "assets/image/bullet-1.png";
inline const char* ExplosionTexturePath = "assets/effect/explosion.png";

struct TextureDeleter {
  void operator()(SDL_Texture* texture) const {
    if (texture) SDL_DestroyTexture(texture);
  }
};
using TexturePtr = std::shared_ptr<SDL_Texture>;

struct Object {
  TexturePtr texture;
  SDL_FPoint pos{0.f, 0.f};
  int width = 0;
  int height = 0;

  void setTexture(SDL_Texture* new_texture) { texture.reset(new_texture, TextureDeleter()); }
  SDL_Texture* getTexture() const { return texture.get(); }
};

struct Player : public Object {
  int speed = PlayerSpeed;                       // pixels per second
  Uint32 bullet_cooldown = PlayerShootCooldown;  // ms
  Uint32 last_shoot_stamp = 0;
  int health = PlayerHealth;
};

struct PlayerBullet : public Object {
  int speed = PlayerBulletSpeed;
  int damage = PlayerBulletDamage;
};

struct Enemy : public Object {
  int speed = EnemySpeed;
  Uint32 last_shoot_stamp = 0;
  Uint32 bullet_cooldown = EnemyShootCooldown;
  int health = EnemyHealth;
  bool isDead = false;
};

struct EnemyBullet : public Object {
  SDL_FPoint direction_vec{0.f, 0.f};
  int speed = EnemyBulletSpeed;
  int damage = EnemyBulletDamage;
};

struct Explosion : public Object {
  Uint32 startTime = 0;
  int totalFrame = 0;
  int currentFrame = 0;
  int FPS = 10;
};

#endif  // ENTITY_HPP__