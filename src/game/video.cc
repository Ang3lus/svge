#include "video.h"

#include <glm/glm.hpp>
#include "third_party/easylogging/easylogging++.h"
#include "third_party/glLoadGen/gl_core_3_3.h"
#include "core/exception.h"
#include "core/sdl_checked_call.h"

namespace svge {
namespace game {

Video::Video() {
  if (SDL_InitSubSystem(SDL_INIT_VIDEO)) {
    throw core::InitFailed("SDL2 intialization failed");
  }

  core::sdl_checked_call(SDL_GL_SetAttribute, SDL_GL_CONTEXT_MAJOR_VERSION, 3);
  core::sdl_checked_call(SDL_GL_SetAttribute, SDL_GL_CONTEXT_MINOR_VERSION, 3);
  core::sdl_checked_call(SDL_GL_SetAttribute, SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
  core::sdl_checked_call(SDL_GL_SetAttribute, SDL_GL_DOUBLEBUFFER, 1);
  core::sdl_checked_call(SDL_GL_SetAttribute, SDL_GL_DEPTH_SIZE, 24);

  SDL_DisplayMode current_display_mode;
  core::sdl_checked_call(SDL_GetCurrentDisplayMode, 0, &current_display_mode);

  window_ = core::sdl_checked_call(SDL_CreateWindow,
    "SVGE",
    SDL_WINDOWPOS_CENTERED,
    SDL_WINDOWPOS_CENTERED,
    current_display_mode.w * 0.75f,
    current_display_mode.h * 0.75f,    SDL_WINDOW_OPENGL
  );

  context_ = core::sdl_checked_call(SDL_GL_CreateContext, window_);

  if (ogl_LOAD_FAILED == ogl_LoadFunctions()) {
    throw core::InitFailed("OpenGL intialization failed");
  }

  voxel_vao_ = core::opengl_helpers::make_smart_vao_unique_ptr();
  glBindVertexArray(*voxel_vao_);
  voxel_vbo_ = core::opengl_helpers::make_smart_vbo_unique_ptr();
  glBindBuffer(GL_ARRAY_BUFFER, *voxel_vbo_);

  std::array<glm::vec3, 8> voxel_vertices = {
    glm::vec3(-1.0, -1.0,  1.0),
    glm::vec3( 1.0, -1.0,  1.0),
    glm::vec3( 1.0,  1.0,  1.0),
    glm::vec3(-1.0,  1.0,  1.0),
    glm::vec3(-1.0, -1.0, -1.0),
    glm::vec3( 1.0, -1.0, -1.0),
    glm::vec3( 1.0,  1.0, -1.0),
    glm::vec3(-1.0,  1.0, -1.0),
  };
	glBufferData(GL_ARRAY_BUFFER, voxel_vertices.size() * sizeof(voxel_vertices[0]), &voxel_vertices[0],
    GL_STATIC_DRAW);

  voxel_ibo_ = core::opengl_helpers::make_smart_vbo_unique_ptr();
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, *voxel_ibo_);
	std::array<GLuint, 36> voxel_indices = {
		// Front
		0, 1, 2,
		2, 3, 0,
		// Top
		1, 5, 6,
		6, 2, 1,
		// Back
		7, 6, 5,
		5, 4, 7,
		// Bottom
		4, 0, 3,
		3, 7, 4,
		// Left
		4, 5, 1,
		1, 0, 4,
		// Right
		3, 2, 6,
		6, 7, 3,
	};
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, voxel_indices.size() * sizeof(voxel_indices[0]), &voxel_indices[0],
    GL_STATIC_DRAW);
}

Video::~Video() {
  SDL_GL_DeleteContext(context_);
  SDL_DestroyWindow(window_);
  SDL_QuitSubSystem(SDL_INIT_VIDEO);
}

void Video::render() {
  glClear(GL_COLOR_BUFFER_BIT);
}

void Video::swap_buffers() {
  SDL_GL_SwapWindow(window_);
}

} // namespace game
} // namespace svge
