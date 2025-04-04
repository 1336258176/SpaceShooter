#include "Game.hpp"

int main(int, char**) {
  auto& game = Game::getInstance();
  game.init();
  game.run();

  return 0;
}