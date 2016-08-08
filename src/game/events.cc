#include "events.h"
#include <SDL.h>
#include "core/exception.h"

namespace svge {
namespace game {

Events::Events() {
  if (SDL_InitSubSystem(SDL_INIT_EVENTS)) {
    throw core::InitFailed("SDL2 intialization failed");
  }
}

Events::~Events() {
  SDL_QuitSubSystem(SDL_INIT_VIDEO);
}

bool Events::poll(Event& event) {
  SDL_Event sdl_event;

  while (SDL_PollEvent(&sdl_event)) {
    switch (sdl_event.type) {
      case SDL_QUIT:
        event.type = Event::Type::kQuit;
        return true;
      default:
        continue;
    }
  }

  return false;
}

} // namespace game
} // namespace svge
