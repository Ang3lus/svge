#include "game.h"
#include <cstdlib>
#include <log4cplus/loggingmacros.h>
#include "scene/scene.h"

namespace svge {
namespace game {

Game::Game(int argc, char* argv[]) : logger_(log4cplus::Logger::getInstance(LOG4CPLUS_TEXT("Game"))) {
  LOG4CPLUS_INFO(logger_, LOG4CPLUS_TEXT("Creating game, bin path: ") << LOG4CPLUS_TEXT(argv[0]));
}

// Just to make compiler happy about all forward declarations
Game::~Game() {}

int Game::start() {
  LOG4CPLUS_INFO(logger_, LOG4CPLUS_TEXT("Game start"));
  if (run_) {
    return EXIT_SUCCESS;
  }

  run_ = true;

  auto last_frame_start = core::time::now();
  while (run_) {
    auto frame_start = core::time::now();

    tick(frame_start - last_frame_start);

    if (create_next_scene_) {
      auto next_scene_uptr = create_next_scene_();
      create_next_scene_ = nullptr;

      // If we do just typeid(*next_scene_uptr) then clang complains:
      // expression with side effects will be evaluated despite being used as an operand to 'typeid'
      const scene::Scene& next_scene = *next_scene_uptr;
      const auto& scene_id = typeid(next_scene);

      active_scene_->exit();
      if (0 == scenes_.count(scene_id)) {
        scenes_[scene_id] = std::move(next_scene_uptr);
        active_scene_ = next_scene_uptr.get();
        active_scene_->load();
      } else {
        active_scene_ = scenes_[scene_id].get();
      }
      active_scene_->enter();
    }

    last_frame_start = frame_start;
  }

  return EXIT_SUCCESS;
}

void Game::stop() {
  LOG4CPLUS_INFO(logger_, LOG4CPLUS_TEXT("Game stop"));
  run_ = false;
}

void Game::tick(const std::chrono::duration<float>& dt) {
  if (active_scene_) {
    active_scene_->update(dt);
    active_scene_->render(dt);
  }
  stop();
}

} // namespace game
} // namespace svge
