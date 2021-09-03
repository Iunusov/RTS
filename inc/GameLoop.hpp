
#pragma once

#include <list>

#include "IObject.hpp"
#include "Scroller.hpp"

class GameLoop final {
public:
  GameLoop() = delete;

  static void Start(const std::list<IObject *> &gameObjects,
                    const Scroller &scroller);
  static void Stop();
};