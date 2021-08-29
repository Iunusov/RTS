#include <atomic>
#include <chrono>
#include <list>
#include <mutex>
#include <random>
#include <thread>

#include "CommandBase.hpp"
#include "Renderer.hpp"
#include "TestObject.hpp"
#include "VideoContext.hpp"

#include "Scroller.hpp"

#undef main

extern const ICommand *command_move;

static std::mutex obj_mutex;
static std::list<IObject *> Objects;

int main(int, char **) {
  VideoContextSDL::Create();
  VideoContextSDL::GetInstance()->setup();
  Renderer2D renderer{VideoContextSDL::GetInstance()};

  Objects.emplace_back(new TestObject(Coord{0, 0}));
  Objects.back()->acceptCommand(*command_move);

  static std::thread t{[]() {
    while (true) {
      {
        const std::lock_guard<std::mutex> lock(obj_mutex);

        for (auto &obj : Objects) {
          obj->execute();
        }
      }
      std::this_thread::sleep_for(
          std::chrono::milliseconds((int)(1000.0 / 10.0)));
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
