#pragma once

#include <vector>

namespace svge {
namespace game {

class Events {
 public:
  struct Event {
    struct Quit {};
    enum class Type {
      kQuit
    };

    Type type;

    union {
      Quit quit;
    };
  };
  Events();
  Events(const Events&) = delete;
  Events& operator=(const Events&) = delete;
  ~Events();

  bool poll(Event& event);
 private:
  std::vector<Event> events_;
};

} // namespace game
} // namespace svge
