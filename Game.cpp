#include <chrono>
#include <vector>

#include "Config.hpp"
#include "GameLoop.hpp"
#include "RenderData.hpp"
#include "Renderer2D.hpp"
#include "Scroller.hpp"
#include "TestObject.hpp"
#include "TestStaticObject.hpp"
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

  for (int i(0); i < 1000; ++i) {
    TestStaticObject *staticobject = new TestStaticObject{};
    staticobject->setPosition(
        Coord{(decltype(Coord::x))gen(rng), (decltype(Coord::y))gen(rng)});
    staticobject->teleportTo(staticobject->getPosition());

    if (staticobject->getPosition().x < 500 ||
        staticobject->getPosition().y < 500)
      continue;
    if (staticobject->getPosition().x >= MAX_COORD - 500 ||
        staticobject->getPosition().y >= MAX_COORD - 500)
      continue;

    Collisions::getInstance()->update_static(*staticobject);
    Objects.emplace_back(staticobject);
  }

  for (size_t i(0); i < MAX_COUNT; i++) {
    IObject *obj = new TestObject();
    obj->setPosition(
        Coord{(decltype(Coord::x))gen(rng), (decltype(Coord::y))gen(rng)});

    while (
        Collisions::getInstance()->checkCollisions(*((IMovableObject *)obj))) {
      static size_t c{};
      if (++c > 10000) {
        std::cout << std::endl
                  << "created " << Objects.size() << " units" << std::endl
                  << std::endl;
        return;
      }
      obj->setPosition(
          Coord{(decltype(Coord::x))gen(rng), (decltype(Coord::y))gen(rng)});
    }

    obj->setHeading((double)(gen(rng) % 360));
    obj->teleportTo(obj->getPosition());

    Collisions::getInstance()->update(*(IMovableObject *)obj);
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
    renderer->Render(scroller.GetPos(), scroller.getScale(), lastRender,
                     timeDiff);

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
