#pragma once

#include <SDL.h>

#include <mutex>

#include "Config.hpp"
#include "Coord.hpp"
#include "IObject.hpp"

#include <iostream>

class Scroller final {
private:
  Coord coord{};
  mutable std::mutex mtx;
  int w{};
  int h{};

public:
  Scroller(int w_, int h_) : w{w_}, h{h_} {}
  Coord GetPos() const noexcept {
    const std::lock_guard<std::mutex> lock(mtx);
    return coord;
  }

  void SetPos(const Coord &pos) noexcept {
    const std::lock_guard<std::mutex> lock(mtx);
    coord = pos;
  }

  void execute() noexcept {
    // Get the next event
    SDL_Event event;

    if (SDL_PollEvent(&event)) {
      if (event.type == SDL_QUIT) {
        SDL_Quit();
        exit(1);
      }

      const std::lock_guard<std::mutex> lock(mtx);
      if (event.type == SDL_KEYDOWN) {
        switch (event.key.keysym.sym) {
        case SDLK_UP:
          coord.y = (coord.y > 100) ? coord.y - 100 : 0;
          break;
        case SDLK_DOWN:
          coord.y = ((coord.y + 100 <= MAX_COORD - h) ? (coord.y + 100)
                                                      : (MAX_COORD - h));
          break;
        case SDLK_LEFT:
          coord.x = (coord.x > 100) ? coord.x - 100 : 0;
          break;
        case SDLK_RIGHT:
          coord.x = ((coord.x + 100 <= MAX_COORD - w) ? (coord.x + 100)
                                                      : (MAX_COORD - w));
          break;
        default:
          break;
        }
      }
    }
  }
};
