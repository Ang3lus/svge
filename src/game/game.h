#pragma once

#include <atomic>
#include <log4cplus/logger.h>
#include <unordered_map>
#include <typeindex>
#include <type_traits>
#include "core/time.h"

namespace svge {
namespace game {

namespace scene {
class Scene;
} // namespace scene

class Game {
 public:
  Game(int argc, char* argv[]);
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
  std::atomic_bool run_{false};
  log4cplus::Logger logger_;

  void tick(const core::time::Delta& dt);
  std::unordered_map<std::type_index, std::unique_ptr<scene::Scene>> scenes_;
  scene::Scene* active_scene_ = nullptr;
  std::function<std::unique_ptr<scene::Scene>()> create_next_scene_;
};

} // namespace game
} // namespace svge
