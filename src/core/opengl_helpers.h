#pragma once

#include <memory>
#include <vector>
#include "third_party/glLoadGen/gl_core_3_3.h"

namespace svge {
namespace core {
namespace opengl_helpers {

struct VaoTraits {
  using value_type = GLuint;
  static value_type create() {
    value_type id = 0;
    glGenVertexArrays(1, &id);
    return id;
  }

  static void destroy(value_type id) {
    glDeleteVertexArrays(1, &id);
  }
};

struct VboTraits {
  using value_type = GLuint;
  static value_type create() {
    value_type id = 0;
    glGenBuffers(1, &id);
    return id;
  }

  static void destroy(value_type id) {
    glDeleteBuffers(1, &id);
  }
};

template<GLenum T>
struct ShaderTraits {
  using value_type = GLuint;
  static value_type create() {
    return glCreateShader(T);
  }

  static void destroy(value_type id) {
    glDeleteShader(id);
  }
};

struct ProgramTraits {
  using value_type = GLuint;
  static value_type create() {
    return glCreateProgram();
  }

  static void destroy(value_type id) {
    glDeleteProgram(id);
  }
};

template<class T>
class OglObject {
 public:
  OglObject() : obj_(T::create()) {}

  // Disable copy and move
  OglObject(const OglObject&) = delete;
  OglObject& operator=(const OglObject&) = delete;

  ~OglObject() {
    T::destroy(obj_);
  }
  operator typename T::value_type() const {
    return obj_;
  }
 private:
  typename T::value_type obj_;
};

struct ShaderSource {
  explicit ShaderSource(const GLchar* src) {
    std::string temp(src);
    data = {temp.begin(), temp.end()};
  }
  ~ShaderSource() = default;
  std::vector<GLchar> data;
};

struct VertexShaderSource : ShaderSource {
  using ShaderSource::ShaderSource;
};
struct FragmentShaderSource : ShaderSource {
  using ShaderSource::ShaderSource;
};

using SmartVao = OglObject<VaoTraits>;
using SmartVaoUPtr = std::unique_ptr<SmartVao>;
using SmartVbo = OglObject<VboTraits>;
using SmartVboUPtr = std::unique_ptr<SmartVbo>;
template<GLenum T>
using SmartShader = OglObject<ShaderTraits<T>>;
using SmartVertexShader = SmartShader<GL_VERTEX_SHADER>;
using SmartVertexShaderUPtr = std::unique_ptr<SmartVertexShader>;
using SmartFragmentShader = SmartShader<GL_FRAGMENT_SHADER>;
using SmartFragmentShaderUPtr = std::unique_ptr<SmartFragmentShader>;
using SmartProgram = OglObject<ProgramTraits>;
using SmartProgramUPtr = std::unique_ptr<SmartProgram>;

class SmartShaderAttacher {
 public:
  template<GLenum T>
  SmartShaderAttacher(const SmartProgram& program, const SmartShader<T>& shader)
    : program_(program),
      shader_(shader) {
        glAttachShader(program_, shader_);
      }

  // Disable copy and move
  SmartShaderAttacher(const SmartShaderAttacher&) = delete;
  SmartShaderAttacher& operator=(const SmartShaderAttacher&) = delete;

  ~SmartShaderAttacher() {
    glDetachShader(program_, shader_);
  };
 private:
  const SmartProgram& program_;
  GLuint shader_;
  void attach() {
    glAttachShader(program_, shader_);
  }
};

SmartVaoUPtr make_smart_vao_unique_ptr();
SmartVboUPtr make_smart_vbo_unique_ptr();
SmartProgramUPtr create_program(const VertexShaderSource& vs_src, const FragmentShaderSource& fs_src);

} // namespace opengl_helpers
} // namespace core
} // namespace svge
