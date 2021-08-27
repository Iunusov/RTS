#include <SDL.h>
#include <SDL_image.h>
#include <SDL_timer.h>

#include <atomic>
#include <chrono>
#include <cstdio>
#include <iostream>
#include <list>
#include <memory>
#include <mutex>
#include <random>
#include <thread>
#include <unordered_map>
#include <vector>
#undef main

#include "CommandBase.hpp"
#include "TestObject.hpp"
#include "VideoContext.hpp"

extern const ICommand *command_move;

inline int64_t carX(int64_t x, int64_t y, int64_t w) {
  return (x - y) * (int64_t)(w / 2.0);
}
inline int64_t carY(int64_t x, int64_t y, int64_t h) {
  return (x + y) * (int64_t)(h / 2.0);
}

std::mutex g_i_mutex; // protects g_i

static std::list<IObject *> Objects;

constexpr const auto MAX_OBJECTS{1000};

void drawMap(SDL_Renderer *rend, Coord coord) noexcept {

  static auto tex = IMG_LoadTexture(rend, "assets/grass.png");

  // connects our texture with dest to control position
  SDL_Rect dest;
  SDL_QueryTexture(tex, NULL, NULL, &dest.w, &dest.h);

  for (int64_t i(0); i < 10; i++) {
    for (int64_t j(0); j < 10; j++) {
      dest.x = (int)(carX(i, j, dest.w) + coord.x);
      dest.y = (int)(carY(i, j, (int64_t)(dest.h / 2.0)) + coord.y);
      SDL_RenderCopy(rend, tex, NULL, &dest);
    }
  }
}

class Scroller {

public:
  std::atomic<int> dir{0};
  Coord coord;

  Scroller() {

    auto th = std::thread([this]() {
      while (1) {
        std::this_thread::sleep_for(
            std::chrono::milliseconds((int)(1000.0 / 60)));
        switch (dir) {
        case 0:
          continue;
          ;
        case 1:
          this->coord.x += 20;
          break;
        case 2:
          this->coord.y += 20;
          break;
        case 3:
          this->coord.x -= 20;
          break;
        case 4:
          this->coord.y -= 20;
          break;
        }
      }
    });

    th.detach();
  }
};

static void addTestObjects(size_t count = MAX_OBJECTS) {
  std::mt19937 rng((unsigned int)time(NULL));
  std::uniform_int_distribution<int64_t> gen(-5000, 5000);
  {
    const std::lock_guard<std::mutex> lock(g_i_mutex);

    for (size_t i(0); i < count; ++i) {
      Objects.emplace_back(new TestObject{Coord{gen(rng), gen(rng), 0}});
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
          std::chrono::milliseconds((int)(1000.0 / 10.0)));
    }
  }};
  t.detach();
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

  {
    const std::lock_guard<std::mutex> lock(g_i_mutex);
    for (auto &obj : Objects) {
      obj->acceptCommand(*command_move);
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
  Scroller scroller{};

  bool close = false;

  // annimation loop
  while (!close) {
    // Get the next event
    SDL_Event event;

    if (SDL_PollEvent(&event)) {
      if (event.type == SDL_QUIT) {
        break;
      }

      if (event.type == SDL_KEYDOWN) {
        switch (event.key.keysym.sym) {

        case SDLK_UP:
          scroller.dir = 2;
          break;
          break;
        case SDLK_DOWN:
          scroller.dir = 4;
          break;
          break;
        case SDLK_LEFT:
          scroller.dir = 1;
          break;
          break;
        case SDLK_RIGHT:
          scroller.dir = 3;
          break;
          break;
        default:
          scroller.dir = 0;
          break;
        }
      } else {
        scroller.dir = 0;
      }
    }

    // clears the screen
    SDL_RenderClear(rend);

    auto coord = scroller.coord;
    drawMap(rend, coord);

    {
      const std::lock_guard<std::mutex> lock(g_i_mutex);
      for (auto &obj : Objects) {
        obj->draw(coord, *VideoContext::GetInstance());
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
