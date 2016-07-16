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

  auto last_frame_start = std::chrono::high_resolution_clock::now();
  while (run_) {
    auto frame_start = std::chrono::high_resolution_clock::now();

    tick(frame_start - last_frame_start);

    last_frame_start = frame_start;
  }

  return EXIT_SUCCESS;
}

void Game::stop() {
  LOG4CPLUS_INFO(logger_, LOG4CPLUS_TEXT("Game stop"));
  run_ = false;
}

void Game::tick(const std::chrono::duration<float>& dt) {
  stop();
}

} // namespace game
} // namespace svge
