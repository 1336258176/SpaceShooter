#include "Renderer.hpp"

Renderer::Renderer(const Windows& window) :
    renderer_(SDL_CreateRenderer(window.get(), -1, SDL_RENDERER_ACCELERATED), DestroyRenderer) {}

void Renderer::setColor(const SDL_Color& color) {
  SDL_SetRenderDrawColor(get(), color.r, color.g, color.b, color.a);
}

void Renderer::renderTexture(SDL_Texture* texture,
                             const SDL_FPoint& pos,
                             int w,
                             int h,
                             SDL_Rect* src) {
  SDL_Rect dst{static_cast<int>(pos.x), static_cast<int>(pos.y), w, h};
  SDL_RenderCopy(renderer_.get(), texture, src, &dst);
}

void Renderer::renderTextureEx(SDL_Texture* texture,
                               const SDL_Rect& dst,
                               double angle,
                               const SDL_Point* center,
                               const SDL_RendererFlip filp) {
  SDL_RenderCopyEx(renderer_.get(), texture, NULL, &dst, angle, center, filp);
}

/**
 * @brief 渲染文本
 *
 * @param font 目标字体
 * @param text 文本
 * @param color 颜色
 * @param dst 目标位置
 * @param hc 水平居中
 * @param vc 竖直居中
 */
void Renderer::renderText(TTF_Font* font,
                          const std::string& text,
                          SDL_Color color,
                          SDL_FPoint& dst,
                          bool hc,
                          bool vc) {
  SDL_Surface* surface = TTF_RenderText_Blended(font, text.c_str(), color);
  SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer_.get(), surface);
  if (hc) {
    dst.x = (Windows::WindowsWidth - surface->w) / 2.f;
  }
  if (vc) {
    dst.y = (Windows::WindowsHeight - surface->h) / 2.f;
  }
  renderTexture(texture, dst, surface->w, surface->h);
  SDL_FreeSurface(surface);
  SDL_DestroyTexture(texture);
}

SDL_Renderer* Renderer::get() { return renderer_.get(); }

void Renderer::present() { SDL_RenderPresent(get()); }

void Renderer::clearScreen() { SDL_RenderClear(get()); }