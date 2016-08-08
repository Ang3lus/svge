#include "opengl_helpers.h"

#include "core/exception.h"

namespace svge {
namespace core {
namespace opengl_helpers {

namespace {

void compile_shader(GLuint shader, const ShaderSource& src) {
  const GLchar* srcs[] = {src.data.data()};
  const GLint lengths[] = {static_cast<GLint>(src.data.size())};
  glShaderSource(shader, 1, srcs, lengths);

  glCompileShader(shader);

  GLint is_compiled = 0;
  glGetShaderiv(shader, GL_COMPILE_STATUS, &is_compiled);
  if(GL_FALSE == is_compiled) {
  	GLint max_length = 0;
  	glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &max_length);

  	std::vector<GLchar> info_log(max_length);
  	glGetShaderInfoLog(shader, max_length, &max_length, &info_log[0]);

    throw core::ShaderCompilationFailed(std::string(info_log.begin(), info_log.end()));
  }
}

}

SmartVaoUPtr make_smart_vao_unique_ptr() {
  return std::make_unique<SmartVao>();
}

SmartVboUPtr make_smart_vbo_unique_ptr() {
  return std::make_unique<SmartVbo>();
}

SmartProgramUPtr create_program(const VertexShaderSource& vs_src, const FragmentShaderSource& fs_src) {
  SmartVertexShader vertex_shader;
  compile_shader(vertex_shader, vs_src);

  SmartFragmentShader fragment_shader;
  compile_shader(fragment_shader, fs_src);

  SmartProgramUPtr program = std::make_unique<SmartProgram>();

  SmartShaderAttacher vertex_shader_attacher(*program, vertex_shader);
  SmartShaderAttacher fragment_shader_attacher(*program, fragment_shader);

	glLinkProgram(*program);

	GLint is_linked = 0;
	glGetProgramiv(*program, GL_LINK_STATUS, (int *)&is_linked);
	if(GL_FALSE == is_linked) {
		GLint max_length = 0;
		glGetProgramiv(*program, GL_INFO_LOG_LENGTH, &max_length);

		std::vector<GLchar> info_log(max_length);
		glGetProgramInfoLog(*program, max_length, &max_length, &info_log[0]);

    throw core::ProgramLinkFailed(std::string(info_log.begin(), info_log.end()));
	}

  return program;
}

} // namespace opengl_helpers
} // namespace core
} // namespace svge
