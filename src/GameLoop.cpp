#include "GameLoop.hpp"

#include <atomic>
#include <chrono>
#include <list>
#include <thread>

#include "IObject.hpp"
#include "RenderData.hpp"

#include "Config.hpp"

namespace {
std::atomic_bool doGameLoop{false};
}

void GameLoop::Start(const std::list<IObject *> &gameObjects) {
  doGameLoop = true;
  static auto th = std::thread([&gameObjects]() {
    doGameLoop = true;
    while (doGameLoop) {
      const auto start = std::chrono::steady_clock::now();
      for (auto &&o : gameObjects) {
        o->execute();
      }

      RenderData::PushRenderingData(gameObjects);
      const auto end = std::chrono::steady_clock::now();
      const size_t spent =
          std::chrono::duration_cast<std::chrono::milliseconds>(end - start)
              .count();
      if (spent >= MODEL_CYCLE_TIME_MS) {
        continue;
      }
      static_assert(MODEL_CYCLE_TIME_MS >= 1, "");
      std::this_thread::sleep_for(
          std::chrono::milliseconds(MODEL_CYCLE_TIME_MS - spent));
    }
  });
  th.detach();
}

void GameLoop::Stop() { doGameLoop = false; }
