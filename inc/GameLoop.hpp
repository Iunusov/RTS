
#pragma once

#include <list>

#include "IObject.hpp"
#include "Renderer.hpp"
#include "Scroller.hpp"

class GameLoop final {
public:
  GameLoop() = delete;

  static void Start(const std::list<IObject *> &gameObjects,
                    const Renderer2D &scroller);
  static void Stop();
};