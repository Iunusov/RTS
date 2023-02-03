#include "VideoContextSDL.hpp"

#include <SDL.h>
#include <SDL_image.h>
#include <SDL_timer.h>
#include <SDL_video.h>

#include <array>
#include <map>
#include <string>

#include "IMovableObject.hpp"
#include "IStaticObject.hpp"

namespace {

constexpr float zoomCameraPosition(double pos, double resolution, double camPos,
                                   double screen_resolution, double scale) {
  return static_cast<float>(pos - resolution / 2 - camPos +
                            (screen_resolution / 2.0) / scale);
}

INLINE void drawTexture(SDL_Renderer *rend, const std::string &path, double x,
                        double y, double heading, double camX, double camY,
                        int w, int h, float scale) {
  static std::map<std::string, SDL_Texture *> textures;
  if (textures.count(path) == 0) {
    textures[path] = IMG_LoadTexture(rend, path.c_str());
  }

  SDL_FRect dest;
  int iw, ih;
  SDL_QueryTexture(textures[path], NULL, NULL, &iw, &ih);
  dest.w = (float)iw;
  dest.h = (float)ih;
  dest.x = zoomCameraPosition(x, dest.w, camX, w, scale);

  dest.y = zoomCameraPosition(y, dest.h, camY, h, scale);

  SDL_RenderTextureRotated(rend, textures[path], NULL, &dest, heading, nullptr,
                           SDL_FLIP_NONE);
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
  if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER | SDL_INIT_EVENTS) != 0) {
    SDL_Log("SDL_Init failed");
    return;
  }

  SDL_Log("Current Driver:");
  std::string driver{SDL_GetCurrentVideoDriver()};
  SDL_Log("%s", driver.c_str());
  SDL_Log("\n");

  const SDL_DisplayMode *DM{SDL_GetCurrentDisplayMode(SDL_GetPrimaryDisplay())};
  auto Width = DM->pixel_w;
  auto Height = DM->pixel_h;
  m_fps = (int)(DM->refresh_rate ? DM->refresh_rate : m_fps);

  SDL_Log("Display Mode:");
  SDL_Log("%d, %d", Width, Height);
  SDL_Log("%d fps", m_fps);
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
  SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1");

  win = SDL_CreateWindow("RTS", 0, 0, DM->pixel_w, DM->pixel_h,
                         SDL_WINDOW_OPENGL);
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

  SDL_GetWindowSize(win, &w, &h);
  SDL_Log("%dx%d", w, h);
}

void VideoContextSDL::draw(const Map &) noexcept {
  static auto tex = IMG_LoadTexture(rend, "assets/grass.jpg");
  if (!tex) {
    return;
  }
  static bool first_time{true};
  static SDL_FRect dest;
  if (first_time) {
    int iw, ih;
    SDL_QueryTexture(tex, NULL, NULL, &iw, &ih);
    dest.w = (float)iw;
    dest.h = (float)ih;
    first_time = false;
  }

  for (int i(0); i < MAX_COORD; i += (int)dest.w) {
    for (int j(0); j < MAX_COORD; j += (int)dest.h) {
      if (!isVisible(Coord{(double)i, (double)j})) {
        continue;
      }
      dest.x =
          (float)(i - cameraPosition.x + (double)(w / 2.0) / double{m_scale});
      dest.y =
          (float)(j - cameraPosition.y + (double)(h / 2.0) / double{m_scale});
      SDL_RenderTexture(rend, tex, NULL, &dest);
    }
  }
}

void VideoContextSDL::draw(const IStaticObject *obj) noexcept {
  drawTexture(rend, "assets/base.png", obj->getPosition().x,
              obj->getPosition().y, 0, cameraPosition.x, cameraPosition.y, w, h,
              m_scale);
}

void VideoContextSDL::draw(const IMovableObject *obj) noexcept {
  drawTexture(rend, "assets/panz.png", obj->getPosition().x,
              obj->getPosition().y, (obj)->getHeading(), cameraPosition.x,
              cameraPosition.y, w, h, m_scale);
  drawTexture(rend, "assets/gun.png", obj->getPosition().x,
              obj->getPosition().y, (obj)->getHeading(), cameraPosition.x,
              cameraPosition.y, w, h, m_scale);
}
