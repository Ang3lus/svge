#pragma once

#include <vector>
#include <glm/glm.hpp>

namespace svge {
namespace game {

struct Voxel {
  glm::vec3 position;
  glm::vec4 color;
};

using Voxels = std::vector<Voxel>;

} // namespace game
} // namespace svge
