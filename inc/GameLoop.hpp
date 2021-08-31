
#pragma once

#include <list>

#include "IObject.hpp"

class GameLoop final {
public:
  GameLoop() = delete;

  static void Start(const std::list<IObject *> &gameObjects);
  static void Stop();
};