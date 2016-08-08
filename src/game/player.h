#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>

namespace svge {
namespace game {

struct Player {
  glm::vec3 position;
  glm::quat rotation;
  const float kHeight = 2;
};

} // namespace game
} // namespace svge
