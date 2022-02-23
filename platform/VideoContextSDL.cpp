#include "VideoContextSDL.hpp"

#include <SDL.h>
#include <SDL_image.h>
#include <SDL_timer.h>
#include <SDL_video.h>

#include <array>
#include <map>

#include "IMovableObject.hpp"
#include "IStaticObject.hpp"

namespace {
const std::array<std::string, 5> bestRenderers = {
    "opengl", "direct3d11", "vulkan", "direct3d", "opengles2"};
std::map<std::string, int> renderPresense;

constexpr int zoomCameraPosition(double pos, int resolution, double camPos,
                                 int screen_resolution, double scale) {
  return static_cast<int>(pos - resolution / 2 - camPos +
                          (int)(screen_resolution / 2.0) / scale);
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

  SDL_DisplayMode DM;
  SDL_GetCurrentDisplayMode(0, &DM);
  auto Width = DM.w;
  auto Height = DM.h;
  m_fps = DM.refresh_rate ? DM.refresh_rate : m_fps;

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

  win =
      SDL_CreateWindow("RTS", 0, 0, DM.w, DM.h,
                       SDL_WINDOW_FULLSCREEN_DESKTOP | SDL_WINDOW_OPENGL |
                           SDL_WINDOW_BORDERLESS | SDL_WINDOW_MAXIMIZED |
                           SDL_WINDOW_INPUT_GRABBED | SDL_WINDOW_ALLOW_HIGHDPI |
                           SDL_WINDOW_MOUSE_CAPTURE);
  if (win == nullptr) {
    SDL_Log("SDL_CreateWindow failed");
    SDL_Quit();
    return;
  }

  int render_idx{-1};

  for (int i = 0; i < SDL_GetNumRenderDrivers(); ++i) {
    SDL_RendererInfo rendererInfo = {};
    SDL_GetRenderDriverInfo(i, &rendererInfo);
    renderPresense[rendererInfo.name] = i;
  }

  for (const auto &rname : bestRenderers) {
    const auto r = renderPresense.find(rname);
    if (r != renderPresense.end()) {
      render_idx = r->second;
      break;
    }
  }

  rend =
      SDL_CreateRenderer(win, render_idx,
                         SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC |
                             SDL_RENDERER_TARGETTEXTURE);

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
  static SDL_Rect dest;
  if (first_time) {
    SDL_QueryTexture(tex, NULL, NULL, &dest.w, &dest.h);
    first_time = false;
  }

  for (int i(0); i < MAX_COORD; i += dest.w) {
    for (int j(0); j < MAX_COORD; j += dest.h) {
      if (!isVisible(Coord{(double)i, (double)j})) {
        continue;
      }
      dest.x =
          (int)(i - (int)cameraPosition.x + (int)(w / 2.0) / double{m_scale});
      dest.y =
          (int)(j - (int)cameraPosition.y + (int)(h / 2.0) / double{m_scale});
      SDL_RenderCopy(rend, tex, NULL, &dest);
    }
  }
}

void VideoContextSDL::draw(const IStaticObject *obj) noexcept {
  static auto tex = IMG_LoadTexture(rend, "assets/base.png");
  if (!tex) {
    return;
  }
  static bool first_time{true};
  static SDL_Rect dest;
  if (first_time) {
    SDL_QueryTexture(tex, NULL, NULL, &dest.w, &dest.h);
    first_time = false;
  }
  dest.x = zoomCameraPosition(obj->getPosition().x, dest.w, cameraPosition.x, w,
                              m_scale);

  dest.y = zoomCameraPosition(obj->getPosition().y, dest.h, cameraPosition.y, h,
                              m_scale);

  SDL_RenderCopyEx(rend, tex, NULL, &dest, 0, nullptr, SDL_FLIP_NONE);
}

void VideoContextSDL::draw(const IMovableObject *obj) noexcept {
  static auto panz = IMG_LoadTexture(rend, "assets/panz.png");
  if (!panz) {
    return;
  }
  static auto gun = IMG_LoadTexture(rend, "assets/gun.png");
  if (!gun) {
    return;
  }
  static bool first_time{true};

  static SDL_Rect dest;

  if (first_time) {
    SDL_QueryTexture(panz, NULL, NULL, &dest.w, &dest.h);
    SDL_QueryTexture(gun, NULL, NULL, &dest.w, &dest.h);
    first_time = false;
  }
  dest.x = zoomCameraPosition(obj->getPosition().x, dest.w, cameraPosition.x, w,
                              m_scale);
  dest.y = zoomCameraPosition(obj->getPosition().y, dest.h, cameraPosition.y, h,
                              m_scale);

  SDL_RenderCopyEx(rend, panz, NULL, &dest,
                   dynamic_cast<const IMovableObject *>(obj)->getHeading(),
                   nullptr, SDL_FLIP_NONE);

  dest.x = zoomCameraPosition(obj->getPosition().x, dest.w, cameraPosition.x, w,
                              m_scale);
  dest.y = zoomCameraPosition(obj->getPosition().y, dest.h, cameraPosition.y, h,
                              m_scale);
  // SDL_RenderCopy(rend, gun, NULL, &dest);

  SDL_RenderCopyEx(rend, gun, NULL, &dest,
                   dynamic_cast<const IMovableObject *>(obj)->getHeading(),
                   nullptr, SDL_FLIP_NONE);
}
