#include "VideoContext.hpp"
#include "IMovableObject.hpp"

#include <SDL.h>
#include <SDL_image.h>
#include <SDL_timer.h>
#include <SDL_video.h>

#include <array>
#include <map>

namespace {
const std::array<std::string, 4> bestRenderers = {"direct3d11", "vulkan",
                                                  "direct3d", "opengl"};
std::map<std::string, int> renderPresense;
} // namespace

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
  if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER) != 0) {
    SDL_Log("SDL_Init failed");
    return;
  }

  SDL_Log("Current Driver:");
  SDL_Log(SDL_GetCurrentVideoDriver());
  SDL_Log("\n");

  SDL_DisplayMode DM;
  SDL_GetCurrentDisplayMode(0, &DM);
  auto Width = DM.w;
  auto Height = DM.h;

  SDL_Log("Display Mode:");
  SDL_Log("%d, %d", Width, Height);
  SDL_Log("\n");

  SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 8);
  SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 8);
  SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 8);
  SDL_GL_SetAttribute(SDL_GL_ALPHA_SIZE, 8);
  SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
  SDL_GL_SetAttribute(SDL_GL_ACCELERATED_VISUAL, 1);
  SDL_GL_SetAttribute(SDL_GL_MULTISAMPLESAMPLES, 1);
  SDL_GL_SetSwapInterval(1);
  SDL_SetHint(SDL_HINT_RENDER_VSYNC, "1");

  win =
      SDL_CreateWindow("RTS", 0, 0, DM.w, DM.h,
                       SDL_WINDOW_FULLSCREEN_DESKTOP | SDL_WINDOW_OPENGL |
                           SDL_WINDOW_BORDERLESS | SDL_WINDOW_MAXIMIZED |
                           SDL_WINDOW_INPUT_GRABBED | SDL_WINDOW_ALLOW_HIGHDPI);
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
  SDL_Log(info.name);
  SDL_Log("\n");
  SDL_Log("Done.");
  SDL_Log("--------------------------------");
  SDL_Log("\n");
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

void VideoContextSDL::draw(const IObject *obj) noexcept {
  static auto panz = IMG_LoadTexture(rend, "assets/panz.png");
  SDL_Rect dest;
  SDL_QueryTexture(panz, NULL, NULL, &dest.w, &dest.h);
  dest.w /= 2;
  dest.h /= 2;
  dest.x = static_cast<int>(obj->getPosition().x - cameraPosition.x);
  dest.y = static_cast<int>(obj->getPosition().y - cameraPosition.y);
  SDL_RenderCopy(rend, panz, NULL, &dest);

  static auto gun = IMG_LoadTexture(rend, "assets/gun.png");
  SDL_QueryTexture(gun, NULL, NULL, &dest.w, &dest.h);
  dest.x = static_cast<int>(obj->getPosition().x - 40 - cameraPosition.x);
  dest.y = static_cast<int>(obj->getPosition().y - cameraPosition.y);
  dest.w /= 2;
  dest.h /= 2;
  // SDL_RenderCopy(rend, gun, NULL, &dest);

  const SDL_Point center{150, 100};

  SDL_RenderCopyEx(rend, gun, NULL, &dest,
                   dynamic_cast<const IMovableObject *>(obj)->fireAngle(),
                   &center, SDL_FLIP_NONE);
}
