#include "src/game.h"
#include "src/strategy.h"
#include <iostream>

using namespace reversi;
using game::Vector2;

int main(void) {
  game::Game game;
  strategy::Points p;
  strategy::Strategy *strategy = &p;
  Vector2 move;

  do {
    std::cout << game << "\n\n";

    std::cout << (game.current == game::Turn::BLACK ? "black" : "white")
              << " to move.\nPossible moves: ";

    auto moves = game.possible_moves();
    for (auto move : moves) {
      std::cout << move << ", ";
    }

    std::cout << std::endl;

    if (game.current == game::Turn::BLACK) {
      do {
        unsigned int x, y;
        std::cout << "x y > ";
        std::cin >> x >> y;
        move.x = x;
        move.y = y;
      } while (moves.find(move) == moves.end());
    } else {
      move = strategy->move(&game);
      std::cout << "White moves " << move << std::endl;
    }
  } while (!game.move(move));

  std::cout << "\n\nblack: " << (int) game.black() << " | white: " <<  (int)game.white()
            << "\n";
  return 0;
}
