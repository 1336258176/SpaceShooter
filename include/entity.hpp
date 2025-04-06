#ifndef ENTITY_HPP__
#define ENTITY_HPP__

#include "pch.hpp"

auto DestroyTexture = [](SDL_Texture* texture) { SDL_DestroyTexture(texture); };

struct Player {
  SDL_Texture* texture = nullptr;
  SDL_FPoint pos{0.f, 0.f};
  int width = 0;
  int height = 0;
  int speed = 300;               // pixels per second
  Uint32 bullet_cooldown = 300;  // ms
  Uint32 last_shoot_stamp = 0;

  ~Player() {
    if (!texture) SDL_DestroyTexture(texture);
  }
};

struct PlayerBullet {
  SDL_Texture* texture = nullptr;
  SDL_FPoint pos{0.f, 0.f};
  int width = 0;
  int height = 0;
  int speed = 600;

  ~PlayerBullet() {
    if (!texture) SDL_DestroyTexture(texture);
  }
};

#endif  // ENTITY_HPP__