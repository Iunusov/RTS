#pragma once

#include "Coord.hpp"

#include <SDL.h>

class Scroller final {
  Coord coord;

public:
  Coord GetPos() const noexcept { return coord; }

  void execute() noexcept {

    // Get the next event
    SDL_Event event;

    if (SDL_PollEvent(&event)) {

      if (event.type == SDL_QUIT) {
        SDL_Quit();
        exit(1);
      }

      Coord tmp(coord);
      if (event.type == SDL_KEYDOWN) {
        switch (event.key.keysym.sym) {

        case SDLK_UP:
          coord.y -= 20;
          break;
        case SDLK_DOWN:
          coord.y += 20;
          break;
        case SDLK_LEFT:
          coord.x -= 20;
          break;
        case SDLK_RIGHT:
          coord.x += 20;
          break;
        default:
          break;
        }
      }
    }
  }
};
