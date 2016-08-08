#pragma once

#include <glm/glm.hpp>

namespace svge {
namespace game {
namespace input {

enum class KayboardKey {
  kA,
  kD,
  kS,
  kW,
  kSpace,
  kLeftCtrl
};

bool key_pressed(KayboardKey key);

glm::ivec2 mouse_position_delta();

} // namespace input
} // namespace game
} // namespace svge
