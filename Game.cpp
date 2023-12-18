#include <chrono>
#include <vector>

#include "Collisions.hpp"
#include "Config.hpp"
#include "GameLoop.hpp"
#include "IObject.hpp"
#include "RenderData.hpp"
#include "Renderer2D.hpp"
#include "Scroller.hpp"
#include "TestObject.hpp"
#include "TestStaticObject.hpp"
#include "VideoContextSDL.hpp"

namespace CommandMove {
extern const ICommand *cmd;
}

#undef main
int main(int, char **) {
  static std::vector<IObject *> Objects;

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

  VideoContextSDL::Create();
  VideoContextSDL::GetInstance()->setup();
  IRenderer *renderer = new Renderer2D{VideoContextSDL::GetInstance()};
  RenderData renderFrame{};
  GameLoop game;
  game.Start(Objects, renderFrame, *renderer);

  while (true) {
    static Scroller scroller{};
    scroller.execute();

    const size_t expectedMS{
        (size_t)(1000.0 / VideoContextSDL::GetInstance()->getFps())};
    const auto start{std::chrono::steady_clock::now()};

    static std::chrono::time_point<std::chrono::steady_clock> timestamp{};
    static std::vector<IObject *> lastRender;
    renderFrame.GetRenderData(lastRender, timestamp);

    renderer->Render(scroller.GetPos(), scroller.getScale(), lastRender,
                     timestamp);

    const auto end{std::chrono::steady_clock::now()};
    const auto elapsedMS{
        (size_t)(std::chrono::duration_cast<std::chrono::milliseconds>(end -
                                                                       start)
                     .count())};

    if (expectedMS > elapsedMS) {
      renderer->Delay((size_t)(expectedMS - elapsedMS));
    }
  }

  return 0;
}
