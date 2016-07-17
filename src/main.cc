#include "game/game.h"

int main(int argc, char* argv[]) {
  svge::game::Game game(argc, argv);

  return game.start();
}
