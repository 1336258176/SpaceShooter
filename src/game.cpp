#include "Game.hpp"

std::unique_ptr<Game> Game::instance_ = nullptr;

Game::Game(Windows&& window, Renderer&& renderer) :
    window_(std::move(window)), renderer_(std::move(renderer)) {}

void Game::init() {
  if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
    shouldColse = true;
    SDL_LogError(SDL_LOG_CATEGORY_ERROR, "SDL init error: %s", SDL_GetError());
  }

  if (IMG_Init(IMG_INIT_PNG) != IMG_INIT_PNG) {
    shouldColse = true;
    SDL_LogError(SDL_LOG_CATEGORY_ERROR, "SDL_image init error: %s", IMG_GetError());
  }

  if (TTF_Init() != 0) {
    shouldColse = true;
    SDL_LogError(SDL_LOG_CATEGORY_ERROR, "SDL_ttf init error: %s", TTF_GetError());
  }

  if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) != 0) {
    shouldColse = true;
    SDL_LogError(SDL_LOG_CATEGORY_ERROR, "SDL_mixer init error: %s", Mix_GetError());
  }

  state = GameState::Start;
  currentScene_ = std::make_unique<GameScene>();
  currentScene_->init();

  nearStar.setTexture(IMG_LoadTexture(renderer_.get(), NearStarBackgroundPath));
  if (!nearStar.texture) {
    SDL_LogError(SDL_LOG_CATEGORY_ERROR, "SDL_image load near-star background texture error");
  }
  SDL_QueryTexture(nearStar.getTexture(), NULL, NULL, &nearStar.width, &nearStar.height);
  nearStar.speed = NearBackgroundSpeed;

  farStar.setTexture(IMG_LoadTexture(renderer_.get(), FarStarBackgroundPath));
  if (!farStar.texture) {
    SDL_LogError(SDL_LOG_CATEGORY_ERROR, "SDL_image load far-star background texture error");
  }
  SDL_QueryTexture(farStar.getTexture(), NULL, NULL, &farStar.width, &farStar.height);
  farStar.speed = FarBackgroundSpeed;
}

void Game::quit() {
  instance_.reset();
  TTF_Quit();
  Mix_CloseAudio();
  Mix_Quit();
  IMG_Quit();
  SDL_Quit();
}

void Game::changeScene(GameState next_game_state) {
  if (next_game_state == GameState::Start) {
    currentScene_.reset(new StartScene());
  } else if (next_game_state == GameState::Running) {
    currentScene_.reset(new GameScene());
  } else if (next_game_state == GameState::GameOver) {
    currentScene_.reset(new EndScene());
  } else {
    SDL_LogError(SDL_LOG_CATEGORY_ERROR, "Error game state!");
  }
  currentScene_->init();
}

Game& Game::getInstance() {
  if (!instance_) {
    Windows window("SpaceShooter");
    Renderer renderer(window);
    instance_ = std::unique_ptr<Game>(new Game(std::move(window), std::move(renderer)));
  }
  return *instance_;
}

SDL_Renderer* Game::getRenderer() { return renderer_.get(); }

SDL_Window* Game::getWindow() { return window_.get(); }

int Game::getWindowHeight() const { return window_.WindowsHeight; }

int Game::getWindowWidth() const { return window_.WindowsWidth; }

int Game::getFPS() const { return static_cast<int>(FPS); }

void Game::run() {
  while (!shouldColse) {
    Uint32 startTime = SDL_GetTicks();

    SDL_Event event;
    handleEvent(event);
    update(deltaTime_);
    present();

    Uint32 endTime = SDL_GetTicks();
    Uint32 diff = endTime - startTime;
    // 实现速度与帧率无关
    if (diff < frameTime_) {
      SDL_Delay(frameTime_ - diff);
      deltaTime_ = frameTime_ / 1000.0f;
    } else {
      deltaTime_ = diff / 1000.0f;
    }
  }
}

void Game::handleEvent(SDL_Event& event) {
  while (SDL_PollEvent(&event)) {
    if (event.type == SDL_QUIT) {
      shouldColse = true;
    } else {
      currentScene_->handleEvent(event);
    }
  }
}

void Game::update(float deltaTime) {
  updateBackground(deltaTime);
  currentScene_->update(deltaTime);
}

void Game::present() {
  renderer_.setColor(SDL_Color{0, 0, 0, 255});
  renderer_.clearScreen();
  renderBackground();
  currentScene_->render();
  renderer_.present();
}

void Game::updateBackground(float deltaTime) {
  nearStar.offset += deltaTime * nearStar.speed;
  if (nearStar.offset >= 0) {
    nearStar.offset -= nearStar.height;
  }
  farStar.offset += deltaTime * farStar.speed;
  if (farStar.offset >= 0) {
    farStar.offset -= farStar.height;
  }
}

void Game::renderBackground() {
  // 将一个texture重复地铺在屏幕上
  for (float pos_y = nearStar.offset; pos_y <= getWindowHeight(); pos_y += nearStar.height) {
    for (float pos_x = 0.f; pos_x <= getWindowWidth(); pos_x += nearStar.width) {
      SDL_FPoint dst;
      dst.x = pos_x;
      dst.y = pos_y;
      renderer_.renderTexture(nearStar.getTexture(), dst, nearStar.width, nearStar.height, NULL);
    }
  }

  for (float pos_y = farStar.offset; pos_y <= getWindowHeight(); pos_y += farStar.height) {
    for (float pos_x = 0.f; pos_x <= getWindowWidth(); pos_x += farStar.width) {
      SDL_FPoint dst;
      dst.x = pos_x;
      dst.y = pos_y;
      renderer_.renderTexture(farStar.getTexture(), dst, farStar.width, farStar.height, NULL);
    }
  }
}
