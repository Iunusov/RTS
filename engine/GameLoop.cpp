#include "GameLoop.hpp"

#include <atomic>
#include <chrono>
#include <thread>
#include <vector>

#include "Collisions.hpp"
#include "Config.hpp"
#include "Coord.hpp"
#include "IObject.hpp"
#include "IRenderer.hpp"
#include "RenderData.hpp"
#include "TestObject.hpp"
#include "TestStaticObject.hpp"

namespace {
std::atomic_bool doGameLoop{false};
}

namespace CommandMove {
extern const ICommand *cmd;
}

void GameLoop::Start(RenderData &frame, IRenderer &renderer) {
  // tmp test data
  for (size_t i(0); i < MAX_COORD; i += 1500) {
    TestStaticObject *obj = new TestStaticObject{};
    obj->setPosition(Coord{(double)i, (double)i});
    obj->teleportTo(obj->getPositionRef());
    Collisions::getInstance()->update_static(*obj);
    Objects.push_back(obj);
  }
  for (size_t i(0); i < MAX_COORD; i += 2500) {
    for (size_t j(0); j < MAX_COORD; j += 2500) {
      TestObject *obj = new TestObject();
      obj->setPosition(Coord{(double)i, (double)j});
      obj->teleportTo(obj->getPositionRef());
      obj->acceptCommand(*CommandMove::cmd);

      Collisions::getInstance()->update(*obj);
      Objects.push_back(obj);
    }
  }
  std::cout << Objects.size() << " objects created" << std::endl;

  // game thread
  doGameLoop = true;
  static auto th = std::thread([this, &frame, &renderer]() {
    doGameLoop = true;
    while (doGameLoop) {
      const auto start = std::chrono::steady_clock::now();
      for (auto o : Objects) {
        o->execute();
      }
      frame.PushRenderingData(Objects, renderer);

      const auto end = std::chrono::steady_clock::now();
      const size_t spent =
          (size_t)std::chrono::duration_cast<std::chrono::milliseconds>(end -
                                                                        start)
              .count();

      if (spent < MODEL_CYCLE_TIME_MS) {
        std::this_thread::sleep_for(
            std::chrono::milliseconds(MODEL_CYCLE_TIME_MS - spent));
      }
    }
  });
  th.detach();
}

void GameLoop::Stop() { doGameLoop = false; }
