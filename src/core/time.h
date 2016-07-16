#pragma once

#include <chrono>

namespace svge {
namespace core {
namespace time {

using TimePoint = std::chrono::high_resolution_clock::time_point;
using Delta = std::chrono::duration<float>;

TimePoint now();

} // namespace time
} // namespace core
} // namespace svge
