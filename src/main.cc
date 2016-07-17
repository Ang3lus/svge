#include "game/game.h"

int main(int argc, char* argv[]) {
  svge::game::Game game(std::make_unique<svge::core::Init>(argc, argv));

  return game.start();
}
