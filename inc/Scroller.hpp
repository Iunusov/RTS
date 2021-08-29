#pragma once

#include <atomic>
#include <thread>

#include "Coord.hpp"

#include <SDL.h>

class Scroller final {
  std::atomic<Coord> coord;

public:
  Coord GetPos() const noexcept { return coord; }

  Scroller() {
    auto th = std::thread([this]() {
      while (1) {
        std::this_thread::sleep_for(
            std::chrono::milliseconds((int)(1000.0 / 6)));
        // Get the next event
        SDL_Event event;

        if (SDL_PollEvent(&event)) {
          Coord tmp(coord);
          if (event.type == SDL_KEYDOWN) {
            switch (event.key.keysym.sym) {

            case SDLK_UP:
              tmp.y -= 20;
              break;
            case SDLK_DOWN:
              tmp.y += 20;
              break;
            case SDLK_LEFT:
              tmp.x -= 20;
              break;
            case SDLK_RIGHT:
              tmp.x += 20;
              break;
            default:
              break;
            }
          }
          coord = tmp;
        }
      }
    });

    th.detach();
  }
};
