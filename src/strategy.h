#pragma once
#include "game.h"

namespace reversi::strategy {
using game::Vector2;

class Strategy {
private:
  virtual long score(game::Game const *state, Vector2 pos) = 0;

public:
  virtual Vector2 move(game::Game const *state);
};

class Points : public Strategy {
  long score(game::Game const *state, Vector2 pos) override;
};
} // namespace reversi::strategy
