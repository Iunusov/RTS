
#pragma once

#include <vector>

#include "Collisions.hpp"
#include "IObject.hpp"
#include "IRenderer.hpp"
#include "RenderData.hpp"

class GameLoop final {
private:
public:
  GameLoop() = default;

  void Start(std::vector<IObject *> &, RenderData &, IRenderer &);
  void Stop();
};