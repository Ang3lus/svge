#pragma once

#include <atomic>
#include <unordered_map>
#include <memory>
#include <functional>
#include <typeindex>
#include <type_traits>
#include "core/init.h"
#include "core/time.h"
#include "video.h"
#include "events.h"

namespace svge {
namespace game {

namespace scene {
class Scene;
} // namespace scene

class Game {
 public:
  Game(std::unique_ptr<core::Init> init);
  ~Game();
  int start();
  void stop();

  template<class T>
  void go_to_scene() {
    static_assert(std::is_base_of<scene::Scene, T>::value, "T has to inherit from Scene!");
    create_next_scene_ = []() {
      return std::make_unique<T>();
    };
  }
 private:
  std::unique_ptr<core::Init> init_;
  std::atomic_bool run_{false};
  std::unordered_map<std::type_index, std::unique_ptr<scene::Scene>> scenes_;
  scene::Scene* active_scene_ = nullptr;
  std::function<std::unique_ptr<scene::Scene>()> create_next_scene_;
  Video video_;
  Events events_;

  void tick(const core::time::Delta& dt);
  void change_scene_if_needed();
};

} // namespace game
} // namespace svge
