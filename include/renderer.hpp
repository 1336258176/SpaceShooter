#ifndef RENDERER_HPP__
#define RENDERER_HPP__

#include "pch.hpp"
#include "windows.hpp"

auto DestroyRenderer = [](SDL_Renderer* renderer) { SDL_DestroyRenderer(renderer); };

class Renderer {
 public:
  Renderer(const Windows& window);

  SDL_Renderer* get();
  void clearScreen();
  void present();
  void setColor(const SDL_Color& color);

 private:
  std::unique_ptr<SDL_Renderer, decltype(DestroyRenderer)> renderer_;
};

#endif  // RENDERER_HPP__