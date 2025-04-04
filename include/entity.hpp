#ifndef ENTITY_HPP__
#define ENTITY_HPP__

#include "pch.hpp"

struct Player {
  SDL_Texture* texture;
  SDL_FPoint pos_;
  int width;
  int height;
};

#endif  // ENTITY_HPP__