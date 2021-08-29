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

std::mutex obj_mutex;
static std::list<IObject *> Objects;

constexpr const auto MAX_OBJECTS{1000};

static void addTestObjects(size_t count = MAX_OBJECTS) {
  std::mt19937 rng((unsigned int)time(NULL));
  std::uniform_int_distribution<int64_t> gen(-5000, 5000);

  for (size_t i(0); i < count; ++i) {
    Objects.emplace_back(new TestObject{Coord{gen(rng), gen(rng), 0}});
  }
}

static void startGameThread() {
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
}

int main(int, char **) {
  VideoContextSDL::Create();
  VideoContextSDL::GetInstance()->setup();
  Renderer2D renderer{VideoContextSDL::GetInstance()};

  Scroller scroller{};

  addTestObjects();

  for (auto &obj : Objects) {
    obj->acceptCommand(*command_move);
  }

  startGameThread();

  while (true) {
    {
      const std::lock_guard<std::mutex> lock(obj_mutex);
      renderer.Render(scroller.GetPos(), Objects);
    }
    renderer.Delay();
  }

  return 0;
}
