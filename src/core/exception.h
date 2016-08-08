#pragma once

#include <stdexcept>

namespace svge {
namespace core {

class InitFailed : public std::runtime_error {
  using std::runtime_error::runtime_error;
};

class SDLCallFailed : public std::runtime_error {
  using std::runtime_error::runtime_error;
};

class ShaderCompilationFailed : public std::runtime_error {
  using std::runtime_error::runtime_error;
};

class ProgramLinkFailed : public std::runtime_error {
  using std::runtime_error::runtime_error;
};

class FailedToGetUniformLocation : public std::runtime_error {
  using std::runtime_error::runtime_error;
};

} // namespace core
} // namespace svge
