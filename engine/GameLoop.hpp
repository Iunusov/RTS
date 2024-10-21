
#pragma once

#include <vector>

#include "IObject.hpp"
class IRenderer;
class RenderData;

class GameLoop final {
private:
  std::vector<IObject *> Objects;

public:
  GameLoop() = default;

  void Start(RenderData &, IRenderer &);
  void Stop();
};