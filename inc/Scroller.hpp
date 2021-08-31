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

        coord.x = coord.x > 1920 ? 1920 : coord.x;
        coord.y = coord.y > 1080 ? 1080 : coord.y;
        coord.x = coord.x < 0 ? 0 : coord.x;
        coord.y = coord.y < 0 ? 0 : coord.y;
      }
    }
  }
};
