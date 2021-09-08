#include <chrono>
#include <vector>

#include "Config.hpp"
#include "GameLoop.hpp"
#include "RenderData.hpp"
#include "Renderer2D.hpp"
#include "Scroller.hpp"
#include "TestObject.hpp"
#include "VideoContextSDL.hpp"

#include "Math.hpp"

#include <random>

namespace CommandMove {
extern const ICommand *cmd;
}

static std::list<IObject *> Objects;
static std::vector<IObject *> lastRender;

static std::mt19937 rng((unsigned int)time(NULL));
static std::uniform_int_distribution<int64_t> gen(0, MAX_COORD);

static void addTestData() {

  constexpr const auto MAX_COUNT{100000}; // 100 000

  for (size_t i(0); i < MAX_COUNT; i++) {
    IObject *obj = new TestObject();
    Coord coord =
        Coord{(decltype(coord.x))gen(rng), (decltype(coord.y))gen(rng)};

    while (Collisions::getInstance()->checkCollisions(coord, obj->getId())) {
      static size_t c{};
      if (++c > 1000) {
        std::cout << std::endl
                  << "created " << Objects.size() << " units" << std::endl
                  << std::endl;
        return;
      }
      coord = Coord{(decltype(coord.x))gen(rng), (decltype(coord.y))gen(rng)};
    }

    obj->setHeading((double)(gen(rng) % 360));
    obj->teleportTo(coord);

    Collisions::getInstance()->update(*obj);
    Objects.emplace_back(obj);
    obj->acceptCommand(*CommandMove::cmd);
  }

  std::cout << std::endl
            << "created " << Objects.size() << " units" << std::endl
            << std::endl;
}

#undef main
int main(int, char **) {
  addTestData();

  VideoContextSDL::Create();
  VideoContextSDL::GetInstance()->setup();
  IRenderer *renderer = new Renderer2D{VideoContextSDL::GetInstance()};

  RenderData renderFrame{};

  GameLoop game;
  game.Start(Objects, renderFrame, *renderer);

  while (true) {
    static Scroller scroller{VideoContextSDL::GetInstance()->getWidth(),
                             VideoContextSDL::GetInstance()->getHeigt()};
    scroller.execute();

    const size_t expectedMS{(size_t)(1000.0 / 60.0)};
    const auto start{std::chrono::steady_clock::now()};

    double timeDiff{};
    renderFrame.GetRenderData(lastRender, timeDiff);
    renderer->Render(scroller.GetPos(), lastRender, timeDiff);

    const auto end{std::chrono::steady_clock::now()};
    const auto elapsedMS{(size_t)(
        std::chrono::duration_cast<std::chrono::milliseconds>(end - start)
            .count())};

    if (expectedMS > elapsedMS) {
      renderer->Delay((size_t)(expectedMS - elapsedMS));
    }
  }

  return 0;
}
