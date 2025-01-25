#include "src/game.h"
#include <iostream>

using reversi::game::Vector2;

int main(void) {
    reversi::game::Game game;
    Vector2 move;

    do {
        std::cout << game << "\n\n";

        std::cout << (game.current == reversi::game::Turn::BLACK ? "black": "white") << " to move.\nPossible moves: ";

        auto moves = game.possible_moves();
        for (auto move : moves) {
            std::cout << move << ", ";
        }

        std::cout << std::endl;

        do {
            unsigned int x, y;
            std::cout << "x y > ";
            std::cin >> x >> y;
            move.x = x;
            move.y = y;
        } while (moves.find(move) == moves.end());

    } while (!game.move(move));

    std::cout << "\n\nblack: " << game.black() << " | white: " << game.white() << "\n";
    return 0;
}
