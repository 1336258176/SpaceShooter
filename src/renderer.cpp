#include "Renderer.hpp"

Renderer::Renderer(const Windows& window)
    : renderer_(SDL_CreateRenderer(window.get(), -1, SDL_RENDERER_ACCELERATED), DestroyRenderer) {}

void Renderer::setColor(const SDL_Color& color) {
  SDL_SetRenderDrawColor(get(), color.r, color.g, color.b, color.a);
}

void Renderer::renderTexture(SDL_Texture* texture, const SDL_FPoint& pos, int w, int h,
                             SDL_Rect* src) {
  SDL_Rect dst{static_cast<int>(pos.x), static_cast<int>(pos.y), w, h};
  SDL_RenderCopy(renderer_.get(), texture, src, &dst);
}

SDL_Renderer* Renderer::get() { return renderer_.get(); }

void Renderer::present() { SDL_RenderPresent(get()); }

void Renderer::clearScreen() { SDL_RenderClear(get()); }