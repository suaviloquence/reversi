#include "strategy.h"
#include <stdexcept>

namespace reversi::strategy {

Vector2 Strategy::move(game::Game const *state) {
  bool stored;
  Vector2 x;
  long score = 0;

  for (Vector2 v : state->possible_moves()) {
    long sc = this->score(state, v);
    if (!stored || score < sc) {
      stored = true;

      x = v;
    }
  }

  if (!stored)
    throw std::runtime_error("no possible moves");
  return x;
}

long Points::score(game::Game const *state, Vector2 pos) {
  long score = 0;
  for (Vector2 d : Vector2::DIRECTIONS) {
    score += state->project(pos, d, state->current);
  }
  return score;
}

} // namespace reversi::strategy
