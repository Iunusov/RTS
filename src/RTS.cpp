#include <SDL.h>
#include <SDL_image.h>
#include <SDL_timer.h>

#include <chrono>
#include <cstdio>
#include <iostream>
#include <memory>
#include <mutex>
#include <random>
#include <thread>
#include <unordered_map>
#include <vector>
#undef main

#include "CommandMove.hpp"
#include "TestObject.hpp"
#include "VideoContext.hpp"

std::mutex g_i_mutex; // protects g_i

static std::vector<std::shared_ptr<IObject>> Objects;

constexpr const auto MAX_OBJECTS{3};

static void addTestObjects(size_t count = MAX_OBJECTS) {
  std::mt19937 rng((unsigned int)time(NULL));
  std::uniform_int_distribution<int64_t> gen(200, 1000);
  {
    Objects.reserve(MAX_OBJECTS);
    const std::lock_guard<std::mutex> lock(g_i_mutex);

    auto x = 500;
    auto y = 500;
    for (size_t i(0); i < count; ++i) {
      auto &&obj = std::make_shared<TestObject>(Coord{x, y, 0});
      Objects.emplace_back(std::move(obj));
    }
  }
}

static void startGameThread() {
  static std::thread t{[]() {
    while (true) {
      {
        const std::lock_guard<std::mutex> lock(g_i_mutex);

        for (auto &obj : Objects) {
          obj->execute();
        }
      }
      std::this_thread::sleep_for(
          std::chrono::milliseconds((int)(1000.0 / 8.0)));
    }
  }};
  t.detach();
}

void drawMap(SDL_Renderer *rend) noexcept {
  // load sample.png into image
  SDL_Surface *surface;
  SDL_RWops *rwop = nullptr;
  rwop = SDL_RWFromFile("assets/grass.png", "rb");
  surface = IMG_LoadPNG_RW(rwop);

  if (!surface) {
    printf("IMG_LoadPNG_RW: %s\n", IMG_GetError());
    // handle error
  }

  SDL_Rect dest;

  static auto tex = SDL_CreateTextureFromSurface(rend, surface);
  // connects our texture with dest to control position
  SDL_QueryTexture(tex, NULL, NULL, &dest.w, &dest.h);

  // sets initial x-position of object
  dest.x = static_cast<int>(300 + 300);

  // sets initial y-position of object
  dest.y = static_cast<int>(300);

  SDL_RenderCopy(rend, tex, NULL, &dest);

  // sets initial x-position of object
  dest.x = static_cast<int>(300 - 300);

  // sets initial y-position of object
  dest.y = static_cast<int>(300);

  SDL_RenderCopy(rend, tex, NULL, &dest);
}

int main(int, char **) {
  {
    SDL_version compiled;
    SDL_version linked;

    SDL_VERSION(&compiled);
    SDL_GetVersion(&linked);
    printf("We compiled against SDL version %d.%d.%d ...\n", compiled.major,
           compiled.minor, compiled.patch);
    printf("But we are linking against SDL version %d.%d.%d.\n", linked.major,
           linked.minor, linked.patch);
  }

  addTestObjects();
  startGameThread();

  CommandMove cmd{Coord{400, 400, 0}};
  {
    const std::lock_guard<std::mutex> lock(g_i_mutex);
    for (auto &&obj : Objects) {
      obj->acceptCommand(cmd);
    }
  }

  // This section is the initialization for SDL2
  if (SDL_Init(SDL_INIT_VIDEO) != 0) {
    std::cout << "SDL_INIT_VIDEO" << std::endl;
    return 1;
  }

  SDL_Window *win = SDL_CreateWindow(
      "RTS", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 1920, 1080,
      SDL_WINDOW_FULLSCREEN_DESKTOP | SDL_WINDOW_OPENGL |
          SDL_WINDOW_BORDERLESS | SDL_WINDOW_MAXIMIZED |
          SDL_WINDOW_INPUT_GRABBED | SDL_WINDOW_ALLOW_HIGHDPI);
  if (win == nullptr) {
    std::cout << "win" << std::endl;
    SDL_Quit();
    return 1;
  }
  // SDL_GL_CreateContext(win);
  // SDL_SetWindowFullscreen(win, SDL_TRUE);

  SDL_Renderer *rend = nullptr;
  for (int i = 0; i < SDL_GetNumRenderDrivers(); ++i) {
    SDL_RendererInfo rendererInfo = {};
    SDL_GetRenderDriverInfo(i, &rendererInfo);

    std::cout << rendererInfo.name << std::endl;

    if (rendererInfo.name != std::string("opengl")) {
      continue;
    }

    rend = SDL_CreateRenderer(win, i,
                              SDL_RENDERER_ACCELERATED |
                                  SDL_RENDERER_PRESENTVSYNC |
                                  SDL_RENDERER_TARGETTEXTURE);
    break;
  }

  VideoContext::Create(rend);

  bool close = false;

  // annimation loop
  while (!close) {
    // Get the next event
    SDL_Event event;
    if (SDL_PollEvent(&event)) {
      if (event.type == SDL_QUIT) {
        break;
      }
    }

    // clears the screen
    SDL_RenderClear(rend);

    {
      const std::lock_guard<std::mutex> lock(g_i_mutex);
      for (auto &&obj : Objects) {
        obj->draw(*VideoContext::GetInstance());
      }
    }

    // triggers the double buffers
    // for multiple rendering
    SDL_RenderPresent(rend);

    // calculates to 60 fps
    SDL_Delay((int)(1000.0 / 60.0));
  }

  // destroy renderer
  SDL_DestroyRenderer(rend);

  // destroy window
  SDL_DestroyWindow(win);

  // close SDL
  SDL_Quit();

  return 0;
}
