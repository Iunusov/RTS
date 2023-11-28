#include "VideoContextSDL.hpp"

#include <SDL.h>
#include <SDL_image.h>
#include <SDL_timer.h>
#include <SDL_video.h>

#include <array>
#include <map>
#include <string>
#include <tuple>

#include "IMovableObject.hpp"
#include "IStaticObject.hpp"
#include "Math.hpp"

namespace {

void drawTexture(SDL_Renderer *rend, const std::string &path,
                        const double x, const double y, const double heading,
                        const double camX, const double camY, const int w,
                        const int h, const double scale, const double timeDiff,
                        const IObject *obj) {

  static std::unordered_map<std::string, std::tuple<SDL_Texture *, SDL_FRect *>>
      textures;

  if (textures.count(path) == 0) {
    auto *dest = new SDL_FRect{};
    int iw, ih;
    auto *texture = IMG_LoadTexture(rend, path.c_str());
    SDL_QueryTexture(texture, NULL, NULL, &iw, &ih);
    dest->w = (float)iw;
    dest->h = (float)ih;
    std::get<0>(textures[path]) = texture;
    std::get<1>(textures[path]) = dest;
  }

  const auto *base = dynamic_cast<const ObjectBase *>(obj);
  const auto approx_position =
      Coord{Math::lerp(base->previousPosition.x, x, timeDiff),
            Math::lerp(base->previousPosition.y, y, timeDiff)};
  auto stored = textures[path];
  auto txt = std::get<0>(stored);
  auto dest = std::get<1>(stored);
  
  dest->x = (float)Math::CalculateScreenPosition(
      approx_position.x - (double)dest->w / 2.0, camX, (double)w,
      scale);

  dest->y = (float)Math::CalculateScreenPosition(
      approx_position.y - (double)dest->h / 2.0, camY, (double)h,
      scale);

  SDL_RenderTextureRotated(rend, txt, NULL,
                           dest,
                           Math::lerp(base->previousHeading, heading, timeDiff),
                           nullptr, SDL_FLIP_NONE);
}

} // namespace

IVideoContext *VideoContextSDL::instance = nullptr;

void VideoContextSDL::Create() noexcept { instance = new VideoContextSDL(); }

VideoContextSDL::~VideoContextSDL() noexcept {
  SDL_DestroyRenderer(rend);
  SDL_DestroyWindow(win);
  SDL_Quit();
}

void VideoContextSDL::clear() noexcept { SDL_RenderClear(rend); }

void VideoContextSDL::delay(size_t ms) const noexcept {
  // calculates to 60 fps
  SDL_Delay((uint32_t)ms);
}

void VideoContextSDL::present() noexcept { SDL_RenderPresent(rend); }

void VideoContextSDL::setup() noexcept {
  SDL_Log("--------------------------------");
  SDL_Log("VideoContextSDL::setup()");
  SDL_Log("--------------------------------");
  SDL_Log("\n");
  if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
    SDL_Log("SDL_Init failed");
    return;
  }

  SDL_Log("Current Driver:");
  std::string driver{SDL_GetCurrentVideoDriver()};
  SDL_Log("%s", driver.c_str());
  SDL_Log("\n");


  SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 8);
  SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 8);
  SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 8);
  SDL_GL_SetAttribute(SDL_GL_ALPHA_SIZE, 8);
  SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
  SDL_GL_SetAttribute(SDL_GL_ACCELERATED_VISUAL, 1);
  SDL_GL_SetAttribute(SDL_GL_MULTISAMPLESAMPLES, 4);
  SDL_GL_SetSwapInterval(1);
  SDL_SetHint(SDL_HINT_RENDER_VSYNC, "1");
  SDL_SetHint(SDL_HINT_GRAB_KEYBOARD, "1");
  SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "2");

  win =  SDL_CreateWindow("RTS", 0,0,  SDL_WINDOW_FULLSCREEN | SDL_WINDOW_OPENGL );
  if(!win) {
	  win = SDL_CreateWindow("RTS", 0,0, SDL_WINDOW_FULLSCREEN );
  }
  if (win == nullptr) {
    SDL_Log("SDL_CreateWindow failed");
    SDL_Quit();
    return;
  }

  rend = SDL_CreateRenderer(
      win, 0, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

  if (rend == nullptr) {
    SDL_Log("SDL_CreateRenderer failed");
    SDL_Quit();
    return;
  }

  SDL_RendererInfo info{};
  SDL_GetRendererInfo(rend, &info);
  SDL_Log("Renderer:");
  SDL_Log("%s", info.name);
  SDL_Log("\n");
  SDL_Log("Done.");
  SDL_Log("--------------------------------");
  SDL_Log("\n");
  SDL_GetCurrentRenderOutputSize(rend, &w, &h);
  SDL_Log("%dx%d", w, h);
}

void VideoContextSDL::draw(const IStaticObject *obj, double timeDiff) noexcept {
  drawTexture(rend, "assets/base.png", obj->getPosition().x,
              obj->getPosition().y, 0, cameraPosition.x, cameraPosition.y, w, h,
              m_scale, timeDiff, obj);
}

void VideoContextSDL::draw(const IMovableObject *obj,
                           double timeDiff) noexcept {
  drawTexture(rend, "assets/panz.png", obj->getPosition().x,
              obj->getPosition().y, (obj)->getHeading(), cameraPosition.x,
              cameraPosition.y, w, h, m_scale, timeDiff, obj);
  drawTexture(rend, "assets/gun.png", obj->getPosition().x,
              obj->getPosition().y, (obj)->getHeading(), cameraPosition.x,
              cameraPosition.y, w, h, m_scale, timeDiff, obj);
}
