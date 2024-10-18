#include "VideoContextSDL.hpp"

#include <SDL.h>
#include <SDL_image.h>
#include <SDL_timer.h>
#include <SDL_video.h>

#include <array>
#include <map>
#include <string>
#include <tuple>
#include <unordered_map>

#include "Math.hpp"
#include "MovableObject.hpp"
#include "StaticObject.hpp"
#include <cmath>

namespace {

void drawTexture(SDL_Renderer *rend, const std::string &path, const double x,
                 const double y, const double heading, const double camX,
                 const double camY, const int w, const int h,
                 const double scale, const double timeDiff,
                 const IObject *obj) {

  static std::unordered_map<std::string, std::tuple<SDL_Texture *, SDL_FRect *>>
      textures;

  if (textures.count(path) == 0) {
    auto *dest = new SDL_FRect{};
    auto *texture = IMG_LoadTexture(rend, path.c_str());
    SDL_GetTextureSize(texture, &dest->w, &dest->h);
    const auto new_texture = std::tuple{texture, dest};
    textures[path] = new_texture;

    std::cout << "texture cached: " << path << std::endl;
  }

  const auto *base = dynamic_cast<const ObjectBase *>(obj);
  const auto approx_position =
      Coord{std::lerp(base->previousPosition.x, x, timeDiff),
            std::lerp(base->previousPosition.y, y, timeDiff)};
  auto stored = textures[path];
  auto txt = std::get<0>(stored);
  auto dest = std::get<1>(stored);

  dest->x = (float)Math::CalculateScreenPosition(approx_position.x, dest->w,
                                                 camX, w, scale);

  dest->y = (float)Math::CalculateScreenPosition(approx_position.y, dest->h,
                                                 camY, h, scale);

  SDL_RenderTextureRotated(rend, txt, NULL, dest,
                           std::lerp(base->previousHeading, heading, timeDiff),
                           nullptr, SDL_FLIP_NONE);
}

} // namespace

IVideoContext *VideoContextSDL::instance = nullptr;

void VideoContextSDL::Create(Window data) noexcept {
  instance = new VideoContextSDL(data);
}

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
  if (!SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS)) {
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
  // SDL_SetHint(SDL_HINT_GRAB_KEYBOARD, "1");
  // SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "2");

  SDL_PropertiesID props = SDL_CreateProperties();

#ifdef _WIN32
  SDL_SetPointerProperty(props, SDL_PROP_WINDOW_CREATE_WIN32_HWND_POINTER,
                         mainWindow);
#else
  SDL_SetNumberProperty(props, SDL_PROP_WINDOW_CREATE_X11_WINDOW_NUMBER,
                        mainWindow);
#endif
  SDL_SetBooleanProperty(props, SDL_PROP_WINDOW_CREATE_FULLSCREEN_BOOLEAN,
                         true);

  win = SDL_CreateWindowWithProperties(props);
  if (win == nullptr) {
    SDL_Log("SDL_CreateWindow failed");
    SDL_Quit();
    return;
  }
  SDL_ShowWindow(win);

  rend = SDL_CreateRenderer(win, 0);

  if (rend == nullptr) {
    SDL_Log("SDL_CreateRenderer failed");
    SDL_Quit();
    return;
  }

  SDL_Log("Renderer:");
  SDL_Log("%s", SDL_GetRendererName(rend));
  SDL_Log("\n");
  SDL_Log("Done.");
  SDL_Log("--------------------------------");
  SDL_Log("\n");
  SDL_GetCurrentRenderOutputSize(rend, &w, &h);
  SDL_Log("%dx%d", w, h);
}

void VideoContextSDL::draw(const StaticObject *obj, double timeDiff) noexcept {
  drawTexture(rend, "assets/base.png", obj->getPositionRef().x,
              obj->getPositionRef().y, 0, cameraPosition.x, cameraPosition.y, w,
              h, (double)m_scale, timeDiff, obj);
}

void VideoContextSDL::draw(const MovableObject *obj, double timeDiff) noexcept {
  const auto &pos{obj->getPositionRef()};
  drawTexture(rend, "assets/panz.png", pos.x, pos.y, (obj)->getHeading(),
              cameraPosition.x, cameraPosition.y, w, h, (double)m_scale,
              timeDiff, obj);
  drawTexture(rend, "assets/gun.png", pos.x, pos.y, (obj)->getHeading(),
              cameraPosition.x, cameraPosition.y, w, h, (double)m_scale,
              timeDiff, obj);
}
