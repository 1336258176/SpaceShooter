#ifndef ENTITY_HPP__
#define ENTITY_HPP__

#include "pch.hpp"

inline auto DestroyTexture = [](SDL_Texture* texture) {
  SDL_DestroyTexture(texture);
};

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

struct Player {
  SDL_Texture* texture = nullptr;
  SDL_FPoint pos{0.f, 0.f};
  int width = 0;
  int height = 0;
  int speed = PlayerSpeed;                       // pixels per second
  Uint32 bullet_cooldown = PlayerShootCooldown;  // ms
  Uint32 last_shoot_stamp = 0;
  int health = PlayerHealth;

  ~Player() {
    if (!texture) SDL_DestroyTexture(texture);
  }
};

struct PlayerBullet {
  SDL_Texture* texture = nullptr;
  SDL_FPoint pos{0.f, 0.f};
  int width = 0;
  int height = 0;
  int speed = PlayerBulletSpeed;
  int damage = PlayerBulletDamage;

  ~PlayerBullet() {
    if (!texture) SDL_DestroyTexture(texture);
  }
};

struct Enemy {
  SDL_Texture* texture = nullptr;
  SDL_FPoint pos{0.f, 0.f};
  int width = 0;
  int height = 0;
  int speed = EnemySpeed;
  Uint32 last_shoot_stamp = 0;
  Uint32 bullet_cooldown = EnemyShootCooldown;
  int health = EnemyHealth;
  bool isDead = false;

  ~Enemy() {
    if (!texture) SDL_DestroyTexture(texture);
  }
};

struct EnemyBullet {
  SDL_Texture* texture = nullptr;
  SDL_FPoint pos{0.f, 0.f};
  SDL_FPoint direction_vec{0.f, 0.f};
  int width = 0;
  int height = 0;
  int speed = EnemyBulletSpeed;
  int damage = EnemyBulletDamage;

  ~EnemyBullet() {
    if (!texture) SDL_DestroyTexture(texture);
  }
};

#endif  // ENTITY_HPP__