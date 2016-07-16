#include "game/game.h"

#include <log4cplus/logger.h>
#include <log4cplus/loggingmacros.h>
#include <log4cplus/configurator.h>

int main(int argc, char* argv[]) {
  log4cplus::BasicConfigurator config;
  config.configure();

  log4cplus::Logger logger = log4cplus::Logger::getInstance(LOG4CPLUS_TEXT("main"));

  svge::game::Game game(argc, argv);

  return game.start();
}
