#include "Scroller.hpp"
#include <SDL.h>

#include "Config.hpp"
#include "Coord.hpp"

namespace {
constexpr const float ZERO_FLOAT{0.0f}, MIN_SCALE{0.1f}, MAX_SCALE{1.7f},
    SCALE_STEP{0.01f}, SCROLL_STEP{150.0f};
}

Coord Scroller::GetCameraPos() const noexcept { return coord; }

void Scroller::SetCameraPos(const Coord &pos) noexcept { coord = pos; }

float Scroller::getScale() const noexcept { return scale; }

void Scroller::execute() noexcept {
  // Get the next event
  SDL_Event event;

  while (SDL_PollEvent(&event)) {
    if (event.type == SDL_EVENT_QUIT) {
      SDL_Quit();
      //exit(1);
    }

    if (event.type == SDL_EVENT_KEY_DOWN) {
      switch (event.key.key) {
      case SDLK_UP:
        coord.y -= (SCROLL_STEP / getScale());
        break;
      case SDLK_DOWN:
        coord.y += (SCROLL_STEP / getScale());
        break;
      case SDLK_LEFT:
        coord.x -= (SCROLL_STEP / getScale());
        break;
      case SDLK_RIGHT:
        coord.x += (SCROLL_STEP / getScale());
        break;
      case SDLK_ESCAPE:
       // exit(0);
      default:
        break;
      }
    }

    else if (event.type == SDL_EVENT_MOUSE_WHEEL) {
      scale += (event.wheel.y > ZERO_FLOAT
                    ? (scale < MAX_SCALE ? SCALE_STEP : ZERO_FLOAT)
                    : (scale > MIN_SCALE ? -SCALE_STEP : ZERO_FLOAT));
    }
  }
}
