#include "strategy.h"
#include "src/game.h"
#include <cstdint>
#include <iostream>
#include <stdexcept>

namespace reversi::strategy {

Vector2 Strategy::move(game::Game const *state) {
  bool stored;
  Vector2 x;
  int64_t score = 0;

  for (Vector2 v : state->possible_moves()) {
    int64_t sc = this->score(state, v);
    std::cout << v << " -> " << sc << '\n';
    if (!stored || score < sc) {
      stored = true;
      x = v;
      score = sc;
    }
  }

  if (!stored)
    throw std::runtime_error("no possible moves");
  return x;
}

int64_t Points::score(game::Game const *state, Vector2 pos) {
  int64_t score = 1;
  for (Vector2 d : Vector2::DIRECTIONS) {
    score += state->project(pos, d, state->current);
  }
  return score;
}

} // namespace reversi::strategy
