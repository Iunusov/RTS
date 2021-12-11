#include "GameLoop.hpp"

#include <atomic>
#include <chrono>
#include <list>
#include <thread>

#include "Config.hpp"
#include "Coord.hpp"
#include "IObject.hpp"
#include "IRenderer.hpp"
#include "RenderData.hpp"

#include <iostream>

namespace {
std::atomic_bool doGameLoop{false};
}

void GameLoop::Start(std::list<IObject *> &gameObjects, RenderData &frame,
                     IRenderer &renderer) {
  doGameLoop = true;
  static auto th = std::thread([&gameObjects, &frame, &renderer]() {
    doGameLoop = true;
    while (doGameLoop) {
      const auto start = std::chrono::steady_clock::now();
      for (auto o : gameObjects) {
        o->execute();
      }
      frame.PushRenderingData(gameObjects, renderer);

      const auto end = std::chrono::steady_clock::now();
      const size_t spent =
          (size_t)std::chrono::duration_cast<std::chrono::milliseconds>(end -
                                                                        start)
              .count();

      {
        size_t cpuLoad =
            (size_t)(100.0 * (double)(spent) / (double(MODEL_CYCLE_TIME_MS)));
        static size_t counter{};
        if ((++counter % 20) == 0) {
          counter = 0;
          std::cout << "cpuLoad: " << cpuLoad << " %" << std::endl;
        }
      }

      if (spent < MODEL_CYCLE_TIME_MS) {
        std::this_thread::sleep_for(
            std::chrono::milliseconds(MODEL_CYCLE_TIME_MS - spent));
      }
    }
  });
  th.detach();
}

void GameLoop::Stop() { doGameLoop = false; }
