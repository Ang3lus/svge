#include "video.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "third_party/easylogging/easylogging++.h"
#include "third_party/glLoadGen/gl_core_3_3.h"
#include "core/exception.h"
#include "core/sdl_checked_call.h"
#include "player.h"

namespace svge {
namespace game {

namespace {
GLint get_uniform_location(const core::opengl_helpers::SmartProgram& program, const std::string& uniform_name) {
  const GLint loc = glGetUniformLocation(program, uniform_name.c_str());

  if (-1 == loc) {
    throw core::FailedToGetUniformLocation(uniform_name);
  }

  return loc;
}

}

Video::Video() {
  if (SDL_InitSubSystem(SDL_INIT_VIDEO)) {
    throw core::InitFailed("SDL2 intialization failed");
  }

  core::sdl_checked_call(SDL_SetRelativeMouseMode, SDL_TRUE);

  core::sdl_checked_call(SDL_GL_SetAttribute, SDL_GL_CONTEXT_MAJOR_VERSION, 3);
  core::sdl_checked_call(SDL_GL_SetAttribute, SDL_GL_CONTEXT_MINOR_VERSION, 3);
  core::sdl_checked_call(SDL_GL_SetAttribute, SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
  core::sdl_checked_call(SDL_GL_SetAttribute, SDL_GL_DOUBLEBUFFER, 1);
  core::sdl_checked_call(SDL_GL_SetAttribute, SDL_GL_DEPTH_SIZE, 24);

  {
    SDL_DisplayMode current_display_mode;
    core::sdl_checked_call(SDL_GetCurrentDisplayMode, 0, &current_display_mode);

    window_ = core::sdl_checked_call(SDL_CreateWindow,
      "SVGE",
      SDL_WINDOWPOS_CENTERED,
      SDL_WINDOWPOS_CENTERED,
      current_display_mode.w * 0.75f,
      current_display_mode.h * 0.75f,    SDL_WINDOW_OPENGL
    );
  }

  context_ = core::sdl_checked_call(SDL_GL_CreateContext, window_);

  if (ogl_LOAD_FAILED == ogl_LoadFunctions()) {
    throw core::InitFailed("OpenGL intialization failed");
  }

  ogl_data_ = std::make_unique<OglData>();

  ogl_data_->voxel_vao = core::opengl_helpers::make_smart_vao_unique_ptr();
  glBindVertexArray(*ogl_data_->voxel_vao);
  ogl_data_->voxel_vbo = core::opengl_helpers::make_smart_vbo_unique_ptr();
  glBindBuffer(GL_ARRAY_BUFFER, *ogl_data_->voxel_vbo);

  std::array<glm::vec3, 8> voxel_vertices = {
    glm::vec3(-0.5, -0.5,  0.5),
    glm::vec3( 0.5, -0.5,  0.5),
    glm::vec3( 0.5,  0.5,  0.5),
    glm::vec3(-0.5,  0.5,  0.5),
    glm::vec3(-0.5, -0.5, -0.5),
    glm::vec3( 0.5, -0.5, -0.5),
    glm::vec3( 0.5,  0.5, -0.5),
    glm::vec3(-0.5,  0.5, -0.5),
  };
	glBufferData(GL_ARRAY_BUFFER, voxel_vertices.size() * sizeof(voxel_vertices[0]), &voxel_vertices[0],
    GL_STATIC_DRAW);

  ogl_data_->voxel_ibo = core::opengl_helpers::make_smart_vbo_unique_ptr();
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, *ogl_data_->voxel_ibo);
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
  glEnableVertexAttribArray(0);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, nullptr);

  ogl_data_->voxel_instance_attributes_vbo = core::opengl_helpers::make_smart_vbo_unique_ptr();
  glBindBuffer(GL_ARRAY_BUFFER, *ogl_data_->voxel_instance_attributes_vbo);
  glEnableVertexAttribArray(1);
  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Voxel),
    reinterpret_cast<void*>(offsetof(Voxel, position)));
  glVertexAttribDivisor(1, 1);

  glEnableVertexAttribArray(2);
  glVertexAttribPointer(2, 4, GL_FLOAT, GL_FALSE, sizeof(Voxel),
    reinterpret_cast<void*>(offsetof(Voxel, color)));
  glVertexAttribDivisor(2, 1);

  core::opengl_helpers::VertexShaderSource vs_src(R"(
    #version 330

    struct Matrices {
      mat4 view;
      mat4 projection;
    };

    uniform Matrices matrices;

    layout (location = 0) in vec3 position;
    layout (location = 1) in vec3 position_offset;
    layout (location = 2) in vec4 color;

    out vec4 vertex_color;

    void main() {
      gl_Position = matrices.projection * matrices.view * vec4(position + position_offset, 1.0f);
      vertex_color = color;
    }
  )");
  core::opengl_helpers::FragmentShaderSource fs_src(R"(
    #version 330

    uniform vec4 color;

    in vec4 vertex_color;
    out vec4 out_color;

    void main() {
      out_color = vertex_color;
    }
  )");
  ogl_data_->voxel_shader = core::opengl_helpers::create_program(vs_src, fs_src);
  glUseProgram(*ogl_data_->voxel_shader);

  ogl_data_->voxel_shader_uniforms.matrices.view =
    get_uniform_location(*ogl_data_->voxel_shader, "matrices.view");

  ogl_data_->voxel_shader_uniforms.matrices.projection =
    get_uniform_location(*ogl_data_->voxel_shader, "matrices.projection");

  {
    int w;
    int h;
    SDL_GL_GetDrawableSize(window_, &w, &h);
    glViewport(0, 0, w, h);

    projection_matrix_ = glm::perspective(90.0f, (w * 1.0f) / h, 0.001f, 1000.0f);
  }

  glUniformMatrix4fv(ogl_data_->voxel_shader_uniforms.matrices.projection, 1, false,
    glm::value_ptr(projection_matrix_));

  glEnable(GL_DEPTH_TEST);
  glEnable(GL_CULL_FACE);
}

Video::~Video() {
  // Delete OpenGL data before shuting OpenGL down
  ogl_data_.reset();

  SDL_GL_DeleteContext(context_);
  SDL_DestroyWindow(window_);
  SDL_QuitSubSystem(SDL_INIT_VIDEO);
}

void Video::render(const Player& player, const Voxels& voxels) {
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  if (voxels.empty()) {
    return;
  }

  glm::mat4 view_matrix = glm::inverse(glm::translate(glm::mat4(), player.position) *
                                       glm::mat4_cast(player.rotation));

  glUniformMatrix4fv(ogl_data_->voxel_shader_uniforms.matrices.view, 1, false,
    glm::value_ptr(view_matrix));

  glBindBuffer(GL_ARRAY_BUFFER, *ogl_data_->voxel_instance_attributes_vbo);
	glBufferData(GL_ARRAY_BUFFER, voxels.size() * sizeof(voxels[0]), &voxels[0], GL_STREAM_DRAW);

  glDrawElementsInstanced(GL_TRIANGLES, 36, GL_UNSIGNED_INT, nullptr, voxels.size());

  {
    const GLenum error = glGetError();
    if (GL_NO_ERROR != error) {
      LOG(ERROR) << "glGetError(): " << error;
    }
  }
}

void Video::swap_buffers() {
  SDL_GL_SwapWindow(window_);
}

} // namespace game
} // namespace svge
