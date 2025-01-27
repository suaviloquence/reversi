#pragma once
#include "game.h"
#include <cstdint>

namespace reversi::strategy {
using game::Vector2;

struct Strategy {
  virtual int64_t score(game::Game const *state, Vector2 pos) = 0;
  virtual Vector2 move(game::Game const *state);
};

struct Points : public Strategy {
  int64_t score(game::Game const *state, Vector2 pos) override;
};
} // namespace reversi::strategy
