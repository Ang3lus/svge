#include "init.h"

#include <SDL.h>
#include "third_party/easylogging/easylogging++.h"
#include "exception.h"

INITIALIZE_EASYLOGGINGPP

namespace svge {
namespace core {

Init::Init(int argc, char* argv[]) : argc_(argc), argv_(argv) {
  START_EASYLOGGINGPP(argc, argv);
  el::Configurations default_config;
  default_config.setToDefault();
  default_config.setGlobally(el::ConfigurationType::Format, "%fbase %line %func %msg");
  el::Loggers::setDefaultConfigurations(default_config, true);

  if (SDL_Init(0)) {
    throw InitFailed("SDL2 intialization failed");
  }
  LOG(INFO);
}

Init::~Init() {
  SDL_Quit();
  LOG(INFO);
}

int Init::argc() const {
  return argc_;
}

char** Init::argv() const {
  return argv_;
}

} // namespace core
} // namespace svge
