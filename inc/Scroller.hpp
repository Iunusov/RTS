#pragma once

#include "Coord.hpp"

#include <SDL.h>

#include <mutex>
class Scroller final {
private:
  Coord coord;
  mutable std::mutex mtx;

public:
  Coord GetPos() const noexcept {

    const std::lock_guard<std::mutex> lock(mtx);
    return coord;
  }

  void execute() noexcept {

    // Get the next event
    SDL_Event event;

    if (SDL_PollEvent(&event)) {

      if (event.type == SDL_QUIT) {
        SDL_Quit();
        exit(1);
      }

      Coord tmp(coord);
      const std::lock_guard<std::mutex> lock(mtx);
      if (event.type == SDL_KEYDOWN) {
        switch (event.key.keysym.sym) {

        case SDLK_UP:
          coord.y -= 50;
          break;
        case SDLK_DOWN:
          coord.y += 50;
          break;
        case SDLK_LEFT:
          coord.x -= 50;
          break;
        case SDLK_RIGHT:
          coord.x += 50;
          break;
        default:
          break;
        }
      }
    }
  }
};
