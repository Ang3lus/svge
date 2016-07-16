#include "game.h"
#include <cstdlib>
#include <log4cplus/loggingmacros.h>

namespace svge {
namespace game {

Game::Game(int argc, char* argv[]) : logger_(log4cplus::Logger::getInstance(LOG4CPLUS_TEXT("Game"))) {
  LOG4CPLUS_INFO(logger_, LOG4CPLUS_TEXT("Creating game, bin path: ") << LOG4CPLUS_TEXT(argv[0]));
}

int Game::start() {
  LOG4CPLUS_INFO(logger_, LOG4CPLUS_TEXT("Game start"));
  if (run_) {
    return EXIT_SUCCESS;
  }

  run_ = true;

  while (run_) {
    stop();
  }

  return EXIT_SUCCESS;
}

void Game::stop() {
  LOG4CPLUS_INFO(logger_, LOG4CPLUS_TEXT("Game stop"));
  run_ = false;
}

} // namespace game
} // namespace svge
