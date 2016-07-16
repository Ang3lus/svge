#pragma once

#include <atomic>
#include <log4cplus/logger.h>

namespace svge {
namespace game {

class Game {
 public:
  Game(int argc, char* argv[]);
  int start();
  void stop();
 private:
  std::atomic_bool run_{false};
  log4cplus::Logger logger_;
};

} // namespace game
} // namespace svge
