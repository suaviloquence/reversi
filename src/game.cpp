#include "game.h"
#include <catch2/catch_test_macros.hpp>
#include <stdexcept>

namespace reversi::game {

Turn operator~(Turn turn) {
  return (turn == Turn::BLACK) ? Turn::WHITE : Turn::BLACK;
}

bool operator==(Cell cell, Turn turn) {
  return (cell == Cell::BLACK && turn == Turn::BLACK) ||
         (cell == Cell::WHITE && turn == Turn::WHITE);
}

bool operator==(Turn turn, Cell cell) { return cell == turn; }

// uint8_t Game::black() const {
//     uint8_t count = 0;
//     for (int8_t i = 0; i < HEIGHT; i++) {
//         for (int8_t j = 0; j < WIDTH; j++) {
//             if (at({.x = j, .y = i}) == Cell::BLACK) count++;
//         }
//     }

//     return count;
// }

// uint8_t Game::white() const {
//     uint8_t count = 0;
//     for (int8_t i = 0; i < HEIGHT; i++) {
//         for (int8_t j = 0; j < WIDTH; j++) {
//             if (at({.x = j, .y = i}) == Cell::WHITE) count++;
//         }
//     }

//     return count;
// }

uint8_t Game::project(Vector2 start, Vector2 d, Turn turn) const {
  if (!contains(start) || at(start) != Cell::EMPTY)
    return 0;
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

  // TODO: reason that these are disjoint, otherwise it's not sound to modify in
  // place
  bool valid = false;
  for (Vector2 d : Vector2::DIRECTIONS) {
    uint8_t n = project(pos, d, current);

    valid |= n > 0;
    for (int8_t i = 1; i <= n; i++) {
      at(pos + d * i) = to_cell(current);
      if (current == Turn::BLACK) {
        black_++;
        white_--;
      } else {
        black_--;
        white_++;
      }
    }
  }

  if (valid) {
    at(pos) = to_cell(current);
    if (current == Turn::BLACK) {
      black_++;
    } else {
      white_++;
    }
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
  return (os << "(" << static_cast<int>(self.x) << ", "
             << static_cast<int>(self.y) << ")");
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

TEST_CASE("game at is correct", "[game]") {
  Game game;

  for (int8_t i = 0; i < HEIGHT; i++) {
    for (int8_t j = 0; j < WIDTH; j++) {
      REQUIRE_NOTHROW(game.at({j, i}));
    }
  }

  REQUIRE_THROWS(game.at({-1, 0}));
  REQUIRE_THROWS(game.at({HEIGHT, 5}));
  REQUIRE_THROWS(game.at({-1, WIDTH}));
}

TEST_CASE("game state changes according to rules", "[game]") {
  Game game;

  SECTION("inital game state is correct") {
    REQUIRE(game.current == Turn::BLACK);

    for (int8_t y = 0; y < HEIGHT; y++) {
      for (int8_t x = 0; x < WIDTH; x++) {
        if (3 <= y && y <= 4 && 3 <= x && x <= 4)
          continue;

        REQUIRE(game.at(Vector2{x, y}) == Cell::EMPTY);
      }
    }

    REQUIRE(game.at({3, 3}) == Cell::WHITE);
    REQUIRE(game.at({3, 4}) == Cell::BLACK);
    REQUIRE(game.at({4, 3}) == Cell::BLACK);
    REQUIRE(game.at({4, 4}) == Cell::WHITE);

    const std::set<Vector2> exp = {{3, 2}, {2, 3}, {5, 4}, {4, 5}};
    REQUIRE(exp == game.possible_moves());
  }

  SECTION("invalid moves throw errors") {
    // out of bounds
    REQUIRE_THROWS(game.move({-1, -1}));
    // not adjacent
    REQUIRE_THROWS(game.move({0, 0}));
    // already contained
    REQUIRE_THROWS(game.move({4, 4}));
  }

  SECTION("state correct after one move") {
    const auto result = game.move({3, 2});
    REQUIRE(!result);
    REQUIRE(game.black() == 4);
    REQUIRE(game.white() == 1);
    REQUIRE(game.at({3, 2}) == Cell::BLACK);
    REQUIRE(game.at({3, 3}) == Cell::BLACK);
    REQUIRE(game.at({3, 4}) == Cell::BLACK);
    REQUIRE(game.at({4, 4}) == Cell::WHITE);
    REQUIRE(game.current == Turn::WHITE);

    const std::set<Vector2> exp = {{2, 2}, {4, 2}, {2, 4}};
    REQUIRE(exp == game.possible_moves());
  }

  SECTION("game finishes") {
      game.at({4, 4}) = Cell::EMPTY;
      game.at({0, 0}) = Cell::BLACK;
      game.at({0, 1}) = Cell::WHITE;
      game.current = Turn::BLACK;

      bool result = game.move({3, 2});
      REQUIRE(!result);
      REQUIRE(game.current == Turn::BLACK);
      result = game.move({0, 2});
      REQUIRE(result);
      REQUIRE(game.black() == 6);
      REQUIRE(game.white() == 0);
  }
}

} // namespace reversi::game
