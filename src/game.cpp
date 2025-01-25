#include "game.h"
#include <stdexcept>

namespace reversi::game {

Turn operator~(Turn turn) {
    return (turn == Turn::BLACK) ? Turn::WHITE : Turn::BLACK;
}

bool operator==(Cell cell, Turn turn) {
    return (cell == Cell::BLACK && turn == Turn::BLACK) || (cell == Cell::WHITE && turn == Turn::WHITE);
}

bool operator==(Turn turn, Cell cell) {
    return cell == turn;
}

uint8_t Game::black() const {
    uint8_t count = 0;
    for (int8_t i = 0; i < HEIGHT; i++) {
        for (int8_t j = 0; j < WIDTH; j++) {
            if (at({.x = j, .y = i}) == Cell::BLACK) count++;
        }
    }

    return count;
}

uint8_t Game::white() const {
    uint8_t count = 0;
    for (int8_t i = 0; i < HEIGHT; i++) {
        for (int8_t j = 0; j < WIDTH; j++) {
            if (at({.x = j, .y = i}) == Cell::WHITE) count++;
        }
    }

    return count;
}

uint8_t Game::project(Vector2 start, Vector2 d, Turn turn) const {
    if (!contains(start) || at(start) != Cell::EMPTY) return 0;
    Vector2 u = start;
    uint8_t count = 0;

    while (contains(u + d) && at(u + d) == ~turn) {
        u = u + d;
        count++;
    }

    if (u != start) {
      if (contains(u + d) && at(u + d) == turn) {
          return count;
      }
    }

    return 0;
}

std::set<Vector2> Game::possible_moves() const {
    std::set<Vector2> moves;

  for (int8_t i = 0; i < HEIGHT; i++) {
      for (int8_t j = 0; j < WIDTH; j++) {
          const Vector2 v{.x = j, .y = i};

          if (at(v) != Cell::EMPTY) {
              continue;
          }

          for (const Vector2 d : Vector2::DIRECTIONS) {
              if (project(v, d, current) > 0) {
                  moves.insert(v);
              }
          }
      }
  }

  return moves;
}

bool Game::move(Vector2 pos) {
    if (!contains(pos)) {
        throw std::out_of_range("move position is invalid");
    }

    // TODO: reason that these are disjoint, otherwise it's not sound to modify in place
    bool valid = false;
    for (Vector2 d : Vector2::DIRECTIONS) {
        uint8_t n = project(pos, d, current);

        valid |= n > 0;
        for (int8_t i = 1; i <= n; i++) {
            at(pos + d * i) = to_cell(current);
        }
    }

    if (valid) {
        at(pos) = to_cell(current);
    } else {
        throw std::out_of_range("move position is invalid");
    }

    current = ~current;

    if (possible_moves().empty()) {
        current = ~current;
        return possible_moves().empty();
    }

    return false;
}

std::ostream &operator<<(std::ostream &os, const Vector2 &self) {
    return (os << "(" <<static_cast<int>(self.x) << ", " << static_cast<int>(self.y)<< ")");
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
