#pragma once

#include <SDL.h>
#include "core/opengl_helpers.h"

namespace svge {
namespace game {

class Video {
 public:
  Video();
  Video(const Video&) = delete;
  Video& operator=(const Video&) = delete;
  ~Video();
  void render();
  void swap_buffers();
 private:
  SDL_Window* window_ = nullptr;
  SDL_GLContext context_ = nullptr;
  core::opengl_helpers::SmartVaoUPtr voxel_vao_;
  core::opengl_helpers::SmartVboUPtr voxel_vbo_;
  core::opengl_helpers::SmartVboUPtr voxel_ibo_;
};

} // namespace game
} // namespace svge
