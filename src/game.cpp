#include "Game.hpp"

std::unique_ptr<Game> Game::instance_ = nullptr;

Game::Game(Windows&& window, Renderer&& renderer) :
    window_(std::move(window)), renderer_(std::move(renderer)) {}

Game::~Game() {
  logger_->warn("game quit");
  quit();
}

void Game::init() {
  // set output logger
  auto now = std::chrono::system_clock::now();
  auto now_time_t = std::chrono::system_clock::to_time_t(now);
  std::tm now_tm;
  localtime_s(&now_tm, &now_time_t);
  std::string log_path = fmt::format("log/{}-{}-{}_{}-{}-{}.log",
                                     now_tm.tm_year + 1900,
                                     now_tm.tm_mon + 1,
                                     now_tm.tm_mday,
                                     now_tm.tm_hour,
                                     now_tm.tm_min,
                                     now_tm.tm_sec);
  logger_ = spdlog::basic_logger_mt("Game", log_path);
  logger_->set_level(spdlog::level::trace);

  // int SDL
  logger_->info("init SDL");
  if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
    shouldColse_ = true;
    logger_->error("SDL init error: {}", SDL_GetError());
  }

  if (IMG_Init(IMG_INIT_PNG) != IMG_INIT_PNG) {
    shouldColse_ = true;
    logger_->error("SDL_image init error: {}", IMG_GetError());
  }

  if (TTF_Init() != 0) {
    shouldColse_ = true;
    logger_->error("SDL_ttf init error: {}", TTF_GetError());
  }

  // read config
  logger_->info("read config file");
  parseConfigFile("config/config.toml");

  // create and init scene
  logger_->info("create and init scene");
  state_ = GameState::Start;
  currentScene_ = std::make_unique<StartScene>();
  currentScene_->init();

  // load background texture
  logger_->info("load background texture");
  nearStar.setTexture(IMG_LoadTexture(renderer_.get(), NearStarBackgroundPath.c_str()));
  if (!nearStar.texture) {
    logger_->error("SDL_image load near-star background texture error");
  }
  SDL_QueryTexture(nearStar.getTexture(), NULL, NULL, &nearStar.width, &nearStar.height);
  nearStar.speed = NearBackgroundSpeed;

  farStar.setTexture(IMG_LoadTexture(renderer_.get(), FarStarBackgroundPath.c_str()));
  if (!farStar.texture) {
    logger_->error("SDL_image load far-star background texture error");
  }
  SDL_QueryTexture(farStar.getTexture(), NULL, NULL, &farStar.width, &farStar.height);
  farStar.speed = FarBackgroundSpeed;

  // load font
  logger_->info("load font");
  title_font_.reset(TTF_OpenFont(TitleFontPath.c_str(), 80));
  if (!title_font_) {
    logger_->error("SDL_ttf open title font error");
  }
  text_font_.reset(TTF_OpenFont(TextFontPath.c_str(), 20));
  if (!text_font_) {
    logger_->error("SDL_ttf open text font error");
  }
}

void Game::quit() {
  instance_.reset();
  TTF_Quit();
  IMG_Quit();
  SDL_Quit();
}

void Game::changeScene(GameState next_game_state) {
  logger_->info("change scene!");
  currentScene_->quit();
  if (next_game_state == GameState::Start) {
    currentScene_.reset(new StartScene());
  } else if (next_game_state == GameState::Running) {
    currentScene_.reset(new GameScene());
  } else if (next_game_state == GameState::GameOver) {
    currentScene_.reset(new EndScene());
  } else {
    logger_->error("Error game state!");
  }
  state_ = next_game_state;
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

TTF_Font* Game::getTitleFont() const { return title_font_.get(); }

TTF_Font* Game::getTextFont() const { return text_font_.get(); }

void Game::setScore(int score) { score_ = score; }

int Game::getScore() const { return score_; }

void Game::close() { shouldColse_ = true; }

void Game::run() {
  while (!shouldColse_) {
    Uint32 startTime = SDL_GetTicks();

    SDL_Event event;
    handleEvent(event);
    update(deltaTime_);
    present();

    Uint32 endTime = SDL_GetTicks();
    Uint32 diff = endTime - startTime;
    // Speed has nothing to do with frame rate
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
      shouldColse_ = true;
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
  // Repeatedly lay a texture on the screen
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
