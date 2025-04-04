#include "Game.hpp"

std::unique_ptr<Game> Game::instance_ = nullptr;

Game::Game(Windows&& window, Renderer&& renderer)
    : window_(std::move(window)), renderer_(std::move(renderer)) {}

void Game::init() {
  if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
    shouldColse = true;
    SDL_LogError(SDL_LOG_CATEGORY_ERROR, "SDL init error: %s", SDL_GetError());
  }

  if (IMG_Init(IMG_INIT_PNG) != IMG_INIT_PNG) {
    shouldColse = true;
    SDL_LogError(SDL_LOG_CATEGORY_ERROR, "SDL_image init error: %s", IMG_GetError());
  }
}

void Game::quit() {
  instance_.reset();
  IMG_Quit();
  SDL_Quit();
}

Game& Game::getInstance() {
  if (!instance_) {
    Windows window("SpaceShooter");
    Renderer renderer(window);
    instance_ = std::unique_ptr<Game>(new Game(std::move(window), std::move(renderer)));
  }
  return *instance_;
}

void Game::run() {
  while (!shouldColse) {
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
      if (event.type == SDL_QUIT) {
        shouldColse = true;
      }
    }
  }
}
