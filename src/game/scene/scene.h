#pragma once

#include "core/time.h"

namespace svge {
namespace game {
namespace scene {

class Scene {
 public:
  virtual ~Scene() = default;
  virtual void load() = 0;
  virtual void enter() = 0;
  virtual void exit() = 0;
  virtual void update(const core::time::Delta& dt) = 0;
  virtual void render(const core::time::Delta& dt) = 0;
};

} // namespace scene
} // namespace game
} // namespace svge
