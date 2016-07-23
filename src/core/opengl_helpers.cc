#include "opengl_helpers.h"

namespace svge {
namespace core {
namespace opengl_helpers {

SmartVaoUPtr make_smart_vao_unique_ptr() {
  return std::make_unique<SmartVao>();
}

SmartVboUPtr make_smart_vbo_unique_ptr() {
  return std::make_unique<SmartVbo>();
}

} // namespace opengl_helpers
} // namespace core
} // namespace svge
