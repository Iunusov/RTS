#include "VideoContext.hpp"

#include <SDL.h>
#include <SDL_image.h>
#include <SDL_timer.h>

#include "TestObject.hpp"

VideoContext *VideoContext::instance = nullptr;

void VideoContext::Create(SDL_Renderer *r) noexcept {
  instance = new VideoContext(r);
}

void VideoContext::draw(const TestObject &obj) noexcept {
  // please provide a path for your image
  static auto surface = SDL_LoadBMP("assets/test.bmp");

  // loads image to our graphics hardware memory.
  static auto tex = SDL_CreateTextureFromSurface(rend, surface);

  // clears main-memory
  // SDL_FreeSurface(surface);

  // let us control our image position
  // so that we can move it with our keyboard.
  SDL_Rect dest;

  // connects our texture with dest to control position
  SDL_QueryTexture(tex, NULL, NULL, &dest.w, &dest.h);

  // adjust height and width of our image box.
  dest.w /= 6;
  dest.h /= 6;

  // sets initial x-position of object
  dest.x = static_cast<int>(obj.getPosition().x);

  // sets initial y-position of object
  dest.y = static_cast<int>(obj.getPosition().y);

  SDL_RenderCopy(rend, tex, NULL, &dest);
}
