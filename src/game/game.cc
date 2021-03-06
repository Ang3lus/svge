#include "game.h"
#include <cstdlib>
#include "third_party/easylogging/easylogging++.h"
#include "scene/scene.h"
#include "scene/gameplay.h"

namespace svge {
namespace game {

Game::Game(std::unique_ptr<core::Init> init) : init_(std::move(init)) {}

// Just to make compiler happy about all forward declarations
Game::~Game() {}

int Game::start() {
  LOG(INFO);

  if (run_) {
    return EXIT_SUCCESS;
  }

  run_ = true;
  auto last_frame_start = core::time::now();

  go_to_scene<scene::Gameplay>();

  while (run_) {
    auto frame_start = core::time::now();
    change_scene_if_needed();
    tick(frame_start - last_frame_start);
    last_frame_start = frame_start;
  }

  return EXIT_SUCCESS;
}

void Game::stop() {
  LOG(INFO);

  run_ = false;
}

void Game::tick(const std::chrono::duration<float>& dt) {
  if (active_scene_) {
    active_scene_->update(dt);
    active_scene_->render(dt, video_);
    video_.swap_buffers();
  }

  {
    Events::Event event;
    while (events_.poll(event)) {
      if (Events::Event::Type::kQuit == event.type) {
        stop();
      }
    }
  }
}

void Game::change_scene_if_needed() {
  if (create_next_scene_) {
    auto next_scene_uptr = create_next_scene_();
    create_next_scene_ = nullptr;

    // If we do just typeid(*next_scene_uptr) then clang complains:
    // expression with side effects will be evaluated despite being used as an operand to 'typeid'
    const scene::Scene& next_scene = *next_scene_uptr;
    const auto& scene_id = typeid(next_scene);

    if (active_scene_) {
      active_scene_->exit();
    }
    if (0 == scenes_.count(scene_id)) {
      active_scene_ = next_scene_uptr.get();
      scenes_[scene_id] = std::move(next_scene_uptr);
      active_scene_->load();
    } else {
      active_scene_ = scenes_[scene_id].get();
    }
    active_scene_->enter();
  }
}

} // namespace game
} // namespace svge
