#include "time.h"

namespace svge {
namespace core {
namespace time {

TimePoint now() {
  return std::chrono::high_resolution_clock::now();
}

} // namespace time
} // namespace core
} // namespace svge
