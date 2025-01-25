#pragma once
#include <cstdint>
#include <ostream>
#include <stdexcept>
#include <vector>
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

struct Vector2 {
  int8_t x, y;

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
    return 0 <= i.y && i.y < HEIGHT && 0 <= i.x && i.x < HEIGHT;
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

  std::vector<Vector2> possible_moves() const;

  friend std::ostream &operator<<(std::ostream &os, const Game &self);
};
} // namespace reversi::game
