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

void GameLoop::Start(std::list<IObject *> &gameObjects, const IRenderer &rend) {
  doGameLoop = true;
  static auto th = std::thread([&gameObjects, &rend, this]() {
    doGameLoop = true;
    while (doGameLoop) {
      const auto start = std::chrono::steady_clock::now();
      for (auto &&o : gameObjects) {
        Collisions::getInstance()->update(*o);

        o->execute();
      }

      RenderData::PushRenderingData(gameObjects, rend);
      const auto end = std::chrono::steady_clock::now();
      const size_t spent =
          std::chrono::duration_cast<std::chrono::milliseconds>(end - start)
              .count();
      // std::cout<<"spent: "<<spent<<"ms"<<std::endl;
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
