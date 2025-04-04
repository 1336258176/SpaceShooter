#include "Renderer.hpp"

Renderer::Renderer(const Windows& window)
    : renderer_(SDL_CreateRenderer(window.get(), -1, SDL_RENDERER_ACCELERATED), DestroyRenderer) {}

void Renderer::setColor(const SDL_Color& color) {
  SDL_SetRenderDrawColor(get(), color.r, color.g, color.b, color.a);
}

SDL_Renderer* Renderer::get() { return renderer_.get(); }

void Renderer::present() { SDL_RenderPresent(get()); }

void Renderer::clearScreen() { SDL_RenderClear(get()); }