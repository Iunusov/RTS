#include "Renderer.hpp"
#include <chrono>
#include <random>
#include <vector>

#include "Config.hpp"
#include "GameLoop.hpp"
#include "RenderData.hpp"
#include "Scroller.hpp"
#include "TestObject.hpp"
#include "VideoContext.hpp"

#include "Math.hpp"

namespace CommandMove {
extern const ICommand *cmd;
}

static std::list<IObject *> Objects;
static std::vector<IObject *> lastRender;

static void addTestData() {
  static std::mt19937 rng((unsigned int)time(NULL));
  static std::uniform_int_distribution<int64_t> gen(0, 30000);
  for (size_t i(0); i < 5000; i++) {
    auto *obj = new TestObject();
    obj->teleportTo(Coord{(double)gen(rng), (double)gen(rng)});
    Objects.emplace_back(obj);
  }
  for (auto o : Objects) {
    o->acceptCommand(*CommandMove::cmd);
  }
}

#undef main
int main(int, char **) {
  addTestData();

  VideoContextSDL::Create();
  VideoContextSDL::GetInstance()->setup();
  Renderer2D renderer{VideoContextSDL::GetInstance()};

  Scroller scroller{};
  GameLoop::Start(Objects, renderer);

  while (true) {
    scroller.execute();

    const auto expectedMS{1000.0 / 60.0};
    const auto start{std::chrono::steady_clock::now()};

    double timeDiff{};
    RenderData::GetRenderData(lastRender, timeDiff);

    renderer.Render(scroller.GetPos(), lastRender, timeDiff);
    const auto end{std::chrono::steady_clock::now()};
    const auto elapsedMS{(size_t)(
        std::chrono::duration_cast<std::chrono::milliseconds>(end - start)
            .count())};

    if (elapsedMS >= expectedMS) {
      continue;
    }

    renderer.Delay((size_t)(expectedMS - elapsedMS));
  }

  return 0;
}
