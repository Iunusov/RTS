
#pragma once

#include <vector>

#include "IObject.hpp"
class IRenderer;
class RenderData;

class GameLoop final {
private:
public:
  GameLoop() = default;

  void Start(std::vector<IObject *> &, RenderData &, IRenderer &);
  void Stop();
};