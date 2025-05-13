#ifndef ENTITY_HPP__
#define ENTITY_HPP__

#include "pch.hpp"

inline int PlayerMaxHP;
inline int PlayerBulletDamage;
inline int PlayerSpeed;
inline int PlayerBulletSpeed;
inline int PlayerShootCooldown;

inline int EnemyMaxHP;
inline int EnemyBulletDamage;
inline int EnemySpeed;
inline int EnemyBulletSpeed;
inline int EnemyShootCooldown;

inline int ItemCollisionCount;
inline int ItemSpeed;
inline int NearBackgroundSpeed;
inline int FarBackgroundSpeed;
inline float GenerateLifeItemProbability;
inline int DefeatEnemyPoints;
inline int CollideEnemyCosts;

inline std::string PlayerTexturePath;
inline std::string PlayerBulletTexturePath;
inline std::string EnemyTexturePath;
inline std::string EnemyBulletTexturePath;
inline std::string ExplosionTexturePath;
inline std::string LifeItemTexturePath;
inline std::string TimeItemTexturePath;
inline std::string ShieldItemTexturePath;
inline std::string NearStarBackgroundPath;
inline std::string FarStarBackgroundPath;
inline std::string HPUITexturePath;

inline std::string TextFontPath;
inline std::string TitleFontPath;

struct TextureDeleter {
  void operator()(SDL_Texture* texture) const {
    if (texture) SDL_DestroyTexture(texture);
  }
};
using TexturePtr = std::shared_ptr<SDL_Texture>;

struct TTFDeleter {
  void operator()(TTF_Font* font) const {
    if (font) TTF_CloseFont(font);
  }
};

struct SurfaceDeleter {
  void operator()(SDL_Surface* surface) const {
    if (surface) SDL_FreeSurface(surface);
  }
};

enum class ItemType { Life, Time, Shield };

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
  int health = PlayerMaxHP;
};

struct PlayerBullet : public Object {
  int speed = PlayerBulletSpeed;
  int damage = PlayerBulletDamage;
};

struct Enemy : public Object {
  int speed = EnemySpeed;
  Uint32 last_shoot_stamp = 0;
  Uint32 bullet_cooldown = EnemyShootCooldown;
  int health = EnemyMaxHP;
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

struct Item : public Object {
  ItemType type = ItemType::Life;
  SDL_FPoint direction_vec{0.f, 0.f};
  int speed = ItemSpeed;
  int CollisionCount = ItemCollisionCount;
};

struct BackGround : public Object {
  float offset = 0.f;  // The offset of the current Texture relative to the window, with a negative
                       // value indicating the part outside the screen
  int speed;
};

struct HPUI : public Object {
  float offset = 0.f;
};

void parseConfigFile(std::string_view file_path);

#endif  // ENTITY_HPP__