#include "gameplay.h"
#include "third_party/easylogging/easylogging++.h"

namespace svge {
namespace game {
namespace scene {

void Gameplay::load() {
  LOG(INFO);
  for (int i = -5; i < 5; ++i) {
    voxels_.push_back({glm::vec3(i, 0, 0), glm::vec4(0, 1, 0, 1)});
  }
}

void Gameplay::enter() {
  LOG(INFO);
}

void Gameplay::exit() {
  LOG(INFO);
}

void Gameplay::update(const core::time::Delta& dt) {
  LOG_AFTER_N(60, INFO) << "60 updates done";
}

void Gameplay::render(const core::time::Delta& dt) {
  LOG_AFTER_N(120, INFO) << "120 renderings done";
}

} // namespace scene
} // namespace game
} // namespace svge
