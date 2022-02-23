#pragma once

#include <SDL.h>

#include <mutex>

#include "Config.hpp"
#include "Coord.hpp"
#include "IObject.hpp"

#include <cmath>
#include <iostream>

class Scroller final {
private:
  double scale{0.8};
  mutable std::mutex mtx;
  const int w{};
  const int h{};
  Coord coord{MAX_COORD / 2.0, MAX_COORD / 2.0};

public:
  Scroller(int w_, int h_) : w{w_}, h{h_} {}
  Coord GetPos() const noexcept {
    // const std::lock_guard<std::mutex> lock(mtx);
    return coord;
  }

  void SetPos(const Coord &pos) noexcept {
    // const std::lock_guard<std::mutex> lock(mtx);
    coord = pos;
  }

  float getScale() const noexcept {
    const std::lock_guard<std::mutex> lock(mtx);
    return (float)pow(scale, 3);
  }

  void execute() noexcept {
    // Get the next event
    SDL_Event event;

    while (SDL_PollEvent(&event)) {
      if (event.type == SDL_QUIT) {
        SDL_Quit();
        exit(1);
      }

      // const std::lock_guard<std::mutex> lock(mtx);

      if (event.type == SDL_KEYDOWN) {
        switch (event.key.keysym.sym) {
        case SDLK_UP:
          coord.y -= (int)(50.0f / getScale());
          break;
        case SDLK_DOWN:
          coord.y += (int)(50.0f / getScale());
          break;
        case SDLK_LEFT:
          coord.x -= (int)(50.0f / getScale());
          break;
        case SDLK_RIGHT:
          coord.x += (int)(50.0f / getScale());
          break;
        case SDLK_ESCAPE:
          exit(0);
        default:
          break;
        }
      }

      else if (event.type == SDL_MOUSEWHEEL) {
        if (event.wheel.y > 0) {
          scale += 0.01;
        } else if (event.wheel.y < 0) {
          if (scale > 0.01)
            scale -= 0.01;
        }
      }
    }
  }
};
