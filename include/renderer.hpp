#ifndef RENDERER_HPP__
#define RENDERER_HPP__

#include "windows.hpp"

auto DestroyRenderer = [](SDL_Renderer* renderer) { SDL_DestroyRenderer(renderer); };

class Renderer final {
 public:
  Renderer(const Windows& window);

  SDL_Renderer* get();
  void clearScreen();
  void present();
  void setColor(const SDL_Color& color);
  void renderTexture(SDL_Texture* texture, const SDL_FPoint& pos, int w, int h,
                     SDL_Rect* src = NULL);

 private:
  std::unique_ptr<SDL_Renderer, decltype(DestroyRenderer)> renderer_;
};

#endif  // RENDERER_HPP__