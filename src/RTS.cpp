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

namespace CommandMove {
extern const ICommand *cmd;
}

static std::list<IObject *> Objects;
static std::vector<IObject *> lastRender;

static void addTestData() {
  constexpr const auto density{400};
  constexpr const auto MAX_COUNT{50000}; // 50 000

  size_t count{0};
  for (size_t i(1); i < MAX_COORD / density; i++) {
    if (count == MAX_COUNT) {
      break;
    }
    for (size_t j(1); j < MAX_COORD / density; j++) {
      if (count == MAX_COUNT) {
        break;
      }
      auto *obj = new TestObject();
      Coord coord = Coord{(decltype(coord.x))(i * density),
                          (decltype(coord.y))(j * density)};
      obj->setHeading((double)(count % 360));
      obj->teleportTo(coord);
      Objects.emplace_back(obj);
      ++count;
    }
  }
  for (auto o : Objects) {
    o->acceptCommand(*CommandMove::cmd);
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

  Scroller scroller{};
  GameLoop game;
  game.Start(Objects, *renderer);

  while (true) {
    scroller.execute();

    const auto expectedMS{1000.0 / 60.0};
    const auto start{std::chrono::steady_clock::now()};

    double timeDiff{};
    RenderData::GetRenderData(lastRender, timeDiff);
    renderer->Render(scroller.GetPos(), lastRender, timeDiff);
    // std::cout << "Rendered objects: " << lastRender.size() << std::endl;

    const auto end{std::chrono::steady_clock::now()};
    const auto elapsedMS{(size_t)(
        std::chrono::duration_cast<std::chrono::milliseconds>(end - start)
            .count())};

    if (elapsedMS >= expectedMS) {
      continue;
    }

    renderer->Delay((size_t)(expectedMS - elapsedMS));
  }

  return 0;
}
