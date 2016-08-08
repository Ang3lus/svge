#pragma once

#include <SDL.h>
#include <glm/glm.hpp>
#include "core/opengl_helpers.h"
#include "voxel.h"

namespace svge {
namespace game {

class Player;

class Video {
 public:
  Video();
  Video(const Video&) = delete;
  Video& operator=(const Video&) = delete;
  ~Video();
  void render(const Player& player, const Voxels& voxels);
  void swap_buffers();
 private:
  SDL_Window* window_ = nullptr;
  SDL_GLContext context_ = nullptr;
  struct OglData {
    core::opengl_helpers::SmartVaoUPtr voxel_vao;
    core::opengl_helpers::SmartVboUPtr voxel_vbo;
    core::opengl_helpers::SmartVboUPtr voxel_ibo;
    core::opengl_helpers::SmartVboUPtr voxel_instance_attributes_vbo;
    core::opengl_helpers::SmartProgramUPtr voxel_shader;

    struct {
      struct {
        GLint view = -1;
        GLint projection = -1;
      } matrices;
    } voxel_shader_uniforms;
  };

  std::unique_ptr<OglData> ogl_data_;
  glm::mat4 projection_matrix_;
};

} // namespace game
} // namespace svge
