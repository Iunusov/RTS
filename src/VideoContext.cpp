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
  static auto panz = IMG_LoadTexture(rend, "assets/panz.png");
  SDL_Rect dest;

  SDL_QueryTexture(panz, NULL, NULL, &dest.w, &dest.h);
  dest.w /=2;
  dest.h /=2;
  dest.x = static_cast<int>(obj.getPosition().x);
  dest.y = static_cast<int>(obj.getPosition().y);
  SDL_RenderCopy(rend, panz, NULL, &dest);

  static auto gun = IMG_LoadTexture(rend, "assets/gun.png");
  SDL_QueryTexture(gun, NULL, NULL, &dest.w, &dest.h);
  dest.x = static_cast<int>(obj.getPosition().x - 40);
  dest.y = static_cast<int>(obj.getPosition().y);
  dest.w /=2;
  dest.h /=2;
  // SDL_RenderCopy(rend, gun, NULL, &dest);

  const SDL_Point center{150, 100};

  SDL_RenderCopyEx(rend, gun, NULL, &dest, (int)obj.fireAngle(), &center,
                   SDL_FLIP_NONE);
}
