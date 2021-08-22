#pragma once

#include <SDL.h>
#include <SDL_image.h>
#include <SDL_timer.h>

#include "Coord.hpp"

class VideoContext final {
private:
  Coord position{0, 0, 0};
  SDL_Renderer *rend = nullptr;

private:
  VideoContext(SDL_Renderer *rend) : rend(rend) {}

  static VideoContext *instance;

public:
  static void Create(SDL_Renderer *r) noexcept {
    instance = new VideoContext(r);
  }
  static VideoContext *GetInstance() noexcept { return instance; }

  void SetPosition(const Coord &pos) noexcept { position = pos; }
  void draw() noexcept {
    // creates a surface to load an image into the main memory
    SDL_Surface *surface;

    // please provide a path for your image
    surface = SDL_LoadBMP("assets/test.bmp");

    SDL_Texture *tex = nullptr;

    // loads image to our graphics hardware memory.
    tex = SDL_CreateTextureFromSurface(rend, surface);

    // clears main-memory
    SDL_FreeSurface(surface);

    // let us control our image position
    // so that we can move it with our keyboard.
    SDL_Rect dest;

    // connects our texture with dest to control position
    SDL_QueryTexture(tex, NULL, NULL, &dest.w, &dest.h);

    // adjust height and width of our image box.
    dest.w /= 6;
    dest.h /= 6;

    // sets initial x-position of object
    dest.x = static_cast<int>(position.x);

    // sets initial y-position of object
    dest.y = static_cast<int>(position.y);

    SDL_RenderCopy(rend, tex, NULL, &dest);
  }
};

inline VideoContext *VideoContext::instance = nullptr;
