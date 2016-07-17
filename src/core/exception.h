#pragma once

#include <stdexcept>

namespace svge {
namespace core {

class InitFailed : public std::runtime_error {
  using std::runtime_error::runtime_error;
};

} // namespace core
} // namespace svge