#include "Renderer.hpp"
#include "TestObject.hpp"
#include "VideoContext.hpp"

#include "Scroller.hpp"

#include "Config.hpp"
#include "GameLoop.hpp"
#include "RenderData.hpp"

#include <chrono>
#include <random>
#include <vector>

namespace CommandMove {
extern const ICommand *cmd;
}

static std::list<IObject *> Objects;
static std::vector<IObject *> lastRender;

static void addTestData() {
  static std::mt19937 rng((unsigned int)time(NULL));
  static std::uniform_int_distribution<int64_t> gen(0, 30000);
  for (size_t i(0); i < 5000; i++) {
    Objects.emplace_back(
        new TestObject(Coord{(double)gen(rng), (double)gen(rng)}));
  }
  for (auto o : Objects) {
    o->acceptCommand(*CommandMove::cmd);
  }
}

#undef main
int main(int, char **) {
  VideoContextSDL::Create();
  VideoContextSDL::GetInstance()->setup();
  Renderer2D renderer{VideoContextSDL::GetInstance()};

  addTestData();
  GameLoop::Start(Objects);

  Scroller scroller{};
  while (true) {
    const auto expectedMS{1000.0 / 60.0};
    const auto start{std::chrono::steady_clock::now()};
    scroller.execute();
    RenderData::GetRenderData(lastRender);
    renderer.Render(scroller.GetPos(), lastRender);
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
