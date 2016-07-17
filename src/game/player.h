#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>

namespace svge {
namespace game {

struct Player {
  glm::vec3 postion;
  glm::quat rotation;
};

} // namespace game
} // namespace svge
