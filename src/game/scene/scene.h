#pragma once

#include "core/time.h"

namespace svge {
namespace game {

class Video;

namespace scene {

class Scene {
 public:
  Scene() = default;
  virtual ~Scene() = default;
  Scene(const Scene&) = delete;
  Scene& operator=(const Scene&) = delete;
  virtual void load() = 0;
  virtual void enter() = 0;
  virtual void exit() = 0;
  virtual void update(const core::time::Delta& dt) = 0;
  virtual void render(const core::time::Delta& dt, Video& video) = 0;
};

} // namespace scene
} // namespace game
} // namespace svge
