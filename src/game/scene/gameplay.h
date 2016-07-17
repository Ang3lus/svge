#pragma once

#include <vector>
#include "scene.h"
#include "game/voxel.h"

namespace svge {
namespace game {
namespace scene {

class Gameplay : public Scene {
 public:
  void load() override;
  void enter() override;
  void exit() override;
  void update(const core::time::Delta& dt) override;
  void render(const core::time::Delta& dt) override;
 private:
  std::vector<Voxel> voxels_;
};

} // namespace scene
} // namespace game
} // namespace svge
