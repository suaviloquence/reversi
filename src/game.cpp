#include "game.h"

namespace reversi::game {

    std::vector<Vector2> Game::possible_moves() const {
        std::vector<Vector2> moves;

        return moves;
    }


std::ostream &operator<<(std::ostream &os, const Game &self) {
  for (auto const &row : self.board) {
    for (Cell cell : row) {
      char c;
      switch (cell) {
      case Cell::BLACK:
        c = 'B';
        break;
      case Cell::WHITE:
        c = 'W';
        break;
      case Cell::EMPTY:
        c = '.';
        break;
      }

      os << c;
    }

    os << '\n';
  }

  return os;
}
} // namespace reversi::game
