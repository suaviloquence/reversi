#pragma once
#include <cstdint>
#include <ostream>
#include <stdexcept>
#include <set>
namespace reversi::game {
constexpr uint8_t WIDTH = 8, HEIGHT = 8;

enum struct Cell {
  EMPTY,
  BLACK,
  WHITE,
};

enum struct Turn {
  BLACK,
  WHITE,
};

Turn operator~(Turn turn);
bool operator==(Cell cell, Turn turn);
bool operator==(Turn turn, Cell cell);

constexpr Cell to_cell(Turn t) {
    return (t == Turn::BLACK) ? Cell::BLACK : Cell::WHITE;
}

struct Vector2 {
  int8_t x, y;

  static const Vector2 DIRECTIONS[];

  constexpr Vector2 operator+(Vector2 rhs) const {
    return Vector2{.x = static_cast<int8_t>(x + rhs.x),
                   .y = static_cast<int8_t>(y + rhs.y)};
  }

  constexpr Vector2 operator-(Vector2 rhs) const {
    return Vector2{.x = static_cast<int8_t>(x - rhs.x),
                   .y = static_cast<int8_t>(y - rhs.y)};
  }

  constexpr Vector2 operator*(int8_t rhs) const {
    return Vector2{.x = static_cast<int8_t>(x * rhs),
                   .y = static_cast<int8_t>(y * rhs)};
  }

  constexpr bool operator==(Vector2 rhs) const {
      return x == rhs.x && y == rhs.y;
  }

  constexpr bool operator!=(Vector2 rhs) const {
      return !(*this == rhs);
  }

  // this is a strict total order, but it's lexicographic,
  // not in the math sense of <, where vectors are incomparable
  constexpr bool operator<(Vector2 rhs) const {
      return x < rhs.x || (x == rhs.x && y < rhs.y);
  }

  friend std::ostream &operator<<(std::ostream &os, const Vector2 &self);
};

struct Game {
  Cell board[HEIGHT][WIDTH];
  Turn current;

  Game(): board{
                {Cell::EMPTY, },
                {Cell::EMPTY, },
                {Cell::EMPTY, },
                {Cell::EMPTY, Cell::EMPTY, Cell::EMPTY, Cell::BLACK, Cell::WHITE, Cell::EMPTY, },
                {Cell::EMPTY, Cell::EMPTY, Cell::EMPTY, Cell::WHITE, Cell::BLACK, Cell::EMPTY, },
                {Cell::EMPTY, },
                {Cell::EMPTY, },
                {Cell::EMPTY, },
            }, current(Turn::BLACK) {}

  constexpr bool contains(Vector2 i) const {
    return 0 <= i.y && i.y < HEIGHT && 0 <= i.x && i.x < WIDTH;
  }

  constexpr Cell &at(Vector2 i) {
    if (!contains(i))
      throw std::out_of_range("board index out of range");
    return board[i.y][i.x];
  }

  constexpr Cell const &at(Vector2 i) const {
    if (!contains(i))
      throw std::out_of_range("board index out of range");
    return board[i.y][i.x];
  }

  // cache later with struct members
  uint8_t black() const;
  uint8_t white() const;

  std::set<Vector2> possible_moves() const;
  uint8_t project(Vector2 start, Vector2 direction, Turn turn) const;
  bool move(Vector2 pos);

  friend std::ostream &operator<<(std::ostream &os, const Game &self);
};

constexpr Vector2 Vector2::DIRECTIONS[] = {
    Vector2{.x = 1, .y = -1},
    Vector2{.x = 1, .y = 0},
    Vector2{.x = 1, .y = 1},
    Vector2{.x = 0, .y = -1},
    Vector2{.x = 0, .y = 1},
    Vector2{.x = -1, .y = -1},
    Vector2{.x = -1, .y = 0},
    Vector2{.x = -1, .y = 1},
};

} // namespace reversi::game
