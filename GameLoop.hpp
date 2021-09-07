
#pragma once

#include <list>

#include "Collisions.hpp"
#include "IObject.hpp"
#include "IRenderer.hpp"
#include "RenderData.hpp"

class GameLoop final {
private:
public:
  GameLoop() = default;

  void Start(std::list<IObject *> &, RenderData &, IRenderer &);
  void Stop();
};