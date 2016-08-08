#include "input.h"

#include <unordered_map>
#include <SDL.h>

namespace svge {
namespace game {
namespace input {

bool key_pressed(KayboardKey key) {
  SDL_PumpEvents();
  const auto& keyboard = SDL_GetKeyboardState(nullptr);

  switch(key) {
    case KayboardKey::kA:        return keyboard[SDL_SCANCODE_A];
    case KayboardKey::kD:        return keyboard[SDL_SCANCODE_D];
    case KayboardKey::kS:        return keyboard[SDL_SCANCODE_S];
    case KayboardKey::kW:        return keyboard[SDL_SCANCODE_W];
    case KayboardKey::kSpace:    return keyboard[SDL_SCANCODE_SPACE];
    case KayboardKey::kLeftCtrl: return keyboard[SDL_SCANCODE_LCTRL];
  }

  return false;
}

glm::ivec2 mouse_position_delta() {
  SDL_PumpEvents();
  glm::ivec2 result;
  SDL_GetRelativeMouseState(&result.x, &result.y);
  return result;
}

} // namespace input
} // namespace game
} // namespace svge
