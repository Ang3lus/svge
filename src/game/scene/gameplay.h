#pragma once

#include "scene.h"

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
};

} // namespace scene
} // namespace game
} // namespace svge
