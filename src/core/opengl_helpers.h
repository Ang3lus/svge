#pragma once

#include <memory>
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
  operator typename T::value_type() {
    return obj_;
  }
 private:
  typename T::value_type obj_;
};

using SmartVao = OglObject<VaoTraits>;
using SmartVaoUPtr = std::unique_ptr<SmartVao>;
using SmartVbo = OglObject<VboTraits>;
using SmartVboUPtr = std::unique_ptr<SmartVbo>;

SmartVaoUPtr make_smart_vao_unique_ptr();
SmartVboUPtr make_smart_vbo_unique_ptr();

} // namespace opengl_helpers
} // namespace core
} // namespace svge
