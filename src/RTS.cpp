#include <atomic>
#include <chrono>
#include <ctime>
#include <iostream>
#include <list>
#include <mutex>
#include <random>
#include <thread>

#include "CommandBase.hpp"
#include "Renderer.hpp"
#include "TestObject.hpp"
#include "VideoContext.hpp"

#include "Config.hpp"
#include "Scroller.hpp"

#undef main

namespace CommandMove {
extern const ICommand *cmd;
}

static std::mutex obj_mutex;
static std::list<IObject *> Objects;

int main(int, char **) {
  VideoContextSDL::Create();
  VideoContextSDL::GetInstance()->setup();
  Renderer2D renderer{VideoContextSDL::GetInstance()};

  Objects.emplace_back(new TestObject(Coord{200, 0}));
  Objects.emplace_back(new TestObject(Coord{0, 600}));
  Objects.emplace_back(new TestObject(Coord{700, 100}));
  for (auto o : Objects) {
    o->acceptCommand(*CommandMove::cmd);
  }

  static std::thread t{[]() {
    while (true) {
      const auto start = std::chrono::steady_clock::now();
      {

        const std::lock_guard<std::mutex> lock(obj_mutex);

        for (auto &obj : Objects) {
          obj->execute();
        }
      }
      const auto end = std::chrono::steady_clock::now();
      const size_t spent =
          std::chrono::duration_cast<std::chrono::microseconds>(end - start)
              .count();

      std::this_thread::sleep_for(
          std::chrono::microseconds(MODEL_CYCLE_TIME_MICROSECONDS - spent));
    }
  }};
  t.detach();

  Scroller scroller{};

  while (true) {
    {
      const std::lock_guard<std::mutex> lock(obj_mutex);
      renderer.Render(scroller.GetPos(), Objects);
    }
    renderer.Delay();
    scroller.execute();
  }

  return 0;
}
