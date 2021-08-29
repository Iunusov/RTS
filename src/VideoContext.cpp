#include "VideoContext.hpp"

#include <SDL.h>
#include <SDL_image.h>
#include <SDL_timer.h>

static inline int64_t carX(int64_t x, int64_t y, int64_t w) {
  return (x - y) * (int64_t)(w / 2.0);
}
static inline int64_t carY(int64_t x, int64_t y, int64_t h) {
  return (x + y) * (int64_t)(h / 2.0);
}

IVideoContext *VideoContextSDL::instance = nullptr;

void VideoContextSDL::Create() noexcept { instance = new VideoContextSDL(); }

VideoContextSDL::~VideoContextSDL() noexcept {
  SDL_DestroyRenderer(rend);
  SDL_DestroyWindow(win);
  SDL_Quit();
}

void VideoContextSDL::clear() noexcept { SDL_RenderClear(rend); }

void VideoContextSDL::delay() const noexcept {
	SDL_Event event;

        if (SDL_PollEvent(&event)) {
          if (event.type == SDL_QUIT) {
            SDL_Quit();
            exit(1);
          }
		}
  // calculates to 60 fps
  SDL_Delay((int)(1000.0 / 60.0));
}

void VideoContextSDL::present() noexcept { SDL_RenderPresent(rend); }

void VideoContextSDL::setup() noexcept {

  if (SDL_Init(SDL_INIT_VIDEO) != 0) {
    return;
  }

  win = SDL_CreateWindow(
      "RTS", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 1920, 1080,
      SDL_WINDOW_FULLSCREEN_DESKTOP | SDL_WINDOW_OPENGL |
          SDL_WINDOW_BORDERLESS | SDL_WINDOW_MAXIMIZED |
          SDL_WINDOW_INPUT_GRABBED | SDL_WINDOW_ALLOW_HIGHDPI);
  if (win == nullptr) {
    SDL_Quit();
    return;
  }

  rend = nullptr;
  for (int i = 0; i < SDL_GetNumRenderDrivers(); ++i) {
    SDL_RendererInfo rendererInfo = {};
    SDL_GetRenderDriverInfo(i, &rendererInfo);
    if (rendererInfo.name != std::string("opengl")) {
      continue;
    }

    rend = SDL_CreateRenderer(win, i,
                              SDL_RENDERER_ACCELERATED |
                                  SDL_RENDERER_PRESENTVSYNC |
                                  SDL_RENDERER_TARGETTEXTURE);
    break;
  }
}

void VideoContextSDL::draw(const Map &) noexcept {
  static auto tex = IMG_LoadTexture(rend, "assets/grass.png");
  SDL_Rect dest;
  SDL_QueryTexture(tex, NULL, NULL, &dest.w, &dest.h);

  for (int64_t i(0); i < 10; i++) {
    for (int64_t j(0); j < 10; j++) {
      dest.x = (int)(carX(i, j, dest.w) - cameraPosition.x);
      dest.y = (int)(carY(i, j, (int64_t)(dest.h / 2.0)) - cameraPosition.y);
      SDL_RenderCopy(rend, tex, NULL, &dest);
    }
  }
}

void VideoContextSDL::draw(const IMovableObject &obj) noexcept {
  static auto panz = IMG_LoadTexture(rend, "assets/panz.png");
  SDL_Rect dest;
  SDL_QueryTexture(panz, NULL, NULL, &dest.w, &dest.h);
  dest.w /= 2;
  dest.h /= 2;
  dest.x = static_cast<int>(obj.getPosition().x - cameraPosition.x);
  dest.y = static_cast<int>(obj.getPosition().y - cameraPosition.y);
  SDL_RenderCopy(rend, panz, NULL, &dest);

  static auto gun = IMG_LoadTexture(rend, "assets/gun.png");
  SDL_QueryTexture(gun, NULL, NULL, &dest.w, &dest.h);
  dest.x = static_cast<int>(obj.getPosition().x - 40 - cameraPosition.x);
  dest.y = static_cast<int>(obj.getPosition().y - cameraPosition.y);
  dest.w /= 2;
  dest.h /= 2;
  // SDL_RenderCopy(rend, gun, NULL, &dest);

  const SDL_Point center{150, 100};

  SDL_RenderCopyEx(rend, gun, NULL, &dest, (int)obj.fireAngle(), &center,
                   SDL_FLIP_NONE);
}
