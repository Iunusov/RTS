
#pragma once

#include <list>

#include "Collisions.hpp"
#include "IObject.hpp"
#include "IRenderer.hpp"
#include "Scroller.hpp"

class GameLoop final {
private:
public:
  GameLoop() = default;

  void Start(std::list<IObject *> &gameObjects, const IRenderer &scroller);
  void Stop();
};