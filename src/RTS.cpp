#include "Renderer.hpp"
#include "TestObject.hpp"
#include "VideoContext.hpp"

#include "Scroller.hpp"

#include "Config.hpp"
#include "GameLoop.hpp"
#include "RenderData.hpp"

namespace CommandMove {
extern const ICommand *cmd;
}

static std::list<IObject *> Objects;
static std::list<IObject *> lastRender;

static void addTestData() {
  Objects.emplace_back(new TestObject(Coord{500, 500}));
  Objects.emplace_back(new TestObject(Coord{0, 60}));
  Objects.emplace_back(new TestObject(Coord{800, 1200}));
  for (auto o : Objects) {
    o->acceptCommand(*CommandMove::cmd);
    break;
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
    RenderData::GetRenderData(lastRender);
    renderer.Render(scroller.GetPos(), lastRender);
    scroller.execute();
    renderer.Delay((size_t)(1000.0 / 60.0));
  }

  return 0;
}
