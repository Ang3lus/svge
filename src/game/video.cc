#include "video.h"

#include <GLFW/glfw3.h>
#include "third_party/easylogging/easylogging++.h"
#include "core/exception.h"

namespace svge {
namespace game {

namespace {
class Init {
 public:
  Init() {
    if (!glfwInit()) {
      throw core::InitFailed("GLFW intialization failed");
    }
    LOG(INFO);
  }
  ~Init() {
    glfwTerminate();
    LOG(INFO);
  }
};
} // namespace;

Video::Video() {
  static Init init;
}

} // namespace game
} // namespace svge
