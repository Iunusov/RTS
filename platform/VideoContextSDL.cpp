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
#include <fstream>

#ifdef _WIN32
extern "C" {
__declspec(dllexport) unsigned long NvOptimusEnablement = 0x00000001;
}
extern "C" {
__declspec(dllexport) int AmdPowerXpressRequestHighPerformance = 1;
}
#endif

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
  int vsync{0};
  SDL_GetRenderVSync(rend, &vsync);
  if (vsync) {
    return;
  }
  SDL_Delay((uint32_t)ms);
}

void VideoContextSDL::present() noexcept { SDL_RenderPresent(rend); }

void VideoContextSDL::setup() noexcept {
  std::string renderer_name;
  std::string vsync_option;
  std::string fps_option;
  std::string antialiasing_option;

  auto renderer_name_file =
      std::ifstream(std::string(SDL_GetBasePath()) + "renderer.txt");

  std::getline(renderer_name_file, renderer_name);
  std::getline(renderer_name_file, vsync_option);
  std::getline(renderer_name_file, fps_option);
  std::getline(renderer_name_file, antialiasing_option);

  if (renderer_name.size() && renderer_name[0] == '#') {
    renderer_name = "";
  }

  SDL_Log(0);
  SDL_Log("-------------------------------------------------");
  SDL_Log("VideoContextSDL::setup()");
  SDL_Log("-------------------------------------------------");
  SDL_Log("%s", SDL_GetRevision());
  SDL_Log(0);
  SDL_Log("Available Drivers:");
  const auto numdrivers{SDL_GetNumRenderDrivers()};
  std::string drivers;

  for (auto i(0); i < numdrivers; ++i) {
    drivers += SDL_GetRenderDriver(i);
    drivers += " ";
  }

  SDL_Log("%s", drivers.c_str());

  if (renderer_name.size()) {
    SDL_SetHint(SDL_HINT_RENDER_DRIVER, renderer_name.c_str());
  }

  if (vsync_option == "1") {
    SDL_SetHint(SDL_HINT_RENDER_VSYNC, "1");
  }

  if (!SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS)) {
    SDL_Log("SDL_Init failed");
    return;
  }
  SDL_Log(0);
  SDL_Log("Current Driver:");
  std::string driver{SDL_GetCurrentVideoDriver()};
  SDL_Log("%s", driver.c_str());

  SDL_PropertiesID props = SDL_CreateProperties();
#ifdef _WIN32
  if (std::string(SDL_GetCurrentVideoDriver()) == "windows") {
    SDL_SetPointerProperty(props, SDL_PROP_WINDOW_CREATE_WIN32_HWND_POINTER,
                           mainWindow);
  }
#endif

#ifdef __linux__
  if (std::string(SDL_GetCurrentVideoDriver()) == "x11") {
    SDL_SetNumberProperty(props, SDL_PROP_WINDOW_CREATE_X11_WINDOW_NUMBER,
                          mainWindow);
  }
#endif
#ifdef __APPLE__
  if (std::string(SDL_GetCurrentVideoDriver()) == "cocoa") {
    SDL_SetPointerProperty(props, SDL_PROP_WINDOW_CREATE_COCOA_WINDOW_POINTER,
                           mainWindow);
  }
#endif
  SDL_SetBooleanProperty(props, SDL_PROP_WINDOW_CREATE_FULLSCREEN_BOOLEAN,
                         true);

  if (renderer_name.find("opengl") != std::string::npos) {
    SDL_SetBooleanProperty(props, SDL_PROP_WINDOW_CREATE_OPENGL_BOOLEAN, true);
  }

  if (renderer_name == "metal") {
    SDL_SetBooleanProperty(props, SDL_PROP_WINDOW_CREATE_METAL_BOOLEAN, true);
  }

  SDL_SetBooleanProperty(props, SDL_PROP_WINDOW_CREATE_HIDDEN_BOOLEAN, true);
  SDL_SetBooleanProperty(props, SDL_PROP_WINDOW_CREATE_MOUSE_GRABBED_BOOLEAN,
                         true);
  SDL_SetBooleanProperty(props, SDL_PROP_WINDOW_CREATE_FOCUSABLE_BOOLEAN, true);
  SDL_Log(0);
  if ((renderer_name.find("opengl") != std::string::npos) &&
      antialiasing_option == "1") {
    SDL_Log("SDL_GL_MULTISAMPLEBUFFERS=1; SDL_GL_MULTISAMPLESAMPLES=4");
    SDL_Log(0);

    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 2);
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    SDL_GL_SetAttribute(SDL_GL_MULTISAMPLEBUFFERS, 1);
    SDL_GL_SetAttribute(SDL_GL_MULTISAMPLESAMPLES, 4);
    SDL_GL_SetAttribute(SDL_GL_ACCELERATED_VISUAL, 1);
  }

  win = SDL_CreateWindowWithProperties(props);
  if (win == nullptr) {
    SDL_Log("SDL_CreateWindow failed");
    SDL_Quit();
    return;
  }

  auto dmode = SDL_GetCurrentDisplayMode(SDL_GetDisplayForWindow(win));

  m_fps = dmode->refresh_rate;

  rend =
      SDL_CreateRenderer(win, renderer_name.size() ? renderer_name.c_str() : 0);

  if (rend == nullptr) {
    SDL_Log("SDL_CreateRenderer failed");
    SDL_Quit();
    return;
  }
  if (vsync_option == "1") {
    SDL_SetRenderVSync(rend, SDL_RENDERER_VSYNC_ADAPTIVE);
  }

  SDL_Log(0);
  SDL_Log("Renderer:");
  SDL_Log("%s", SDL_GetRendererName(rend));
  SDL_Log(0);
  SDL_GetCurrentRenderOutputSize(rend, &w, &h);
  SDL_Log("%dx%d", w, h);
  SDL_Log(0);
  SDL_Log("VSync: ");
  int vsync{0};
  SDL_GetRenderVSync(rend, &vsync);

  if (vsync) {
    SDL_Log("on");
  } else {
    SDL_Log("off");
  }

  if (fps_option.size() && !vsync) {
    m_fps = round(std::stoi(fps_option));
  }

  SDL_Log("FPS: %f", m_fps);

  SDL_ShowWindow(win);
  SDL_Log("-------------------------------------------------");
  SDL_Log("VideoContextSDL::setup() finished successfully");
  SDL_Log("-------------------------------------------------");
  SDL_Log(0);
  SDL_Log(0);
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
