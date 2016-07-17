#pragma once

namespace svge {
namespace core {

class Init {
 public:
  Init(int argc, char* argv[]);
  ~Init();
  int argc() const;
  char** argv() const;
 private:
  int argc_ = 0;
  char** argv_ = nullptr;
};

} // namespace core
} // namespace svge

