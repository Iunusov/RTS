#include <list>

#include "Renderer.hpp"
#include "TestObject.hpp"
#include "VideoContext.hpp"

#include "Scroller.hpp"

#include "Config.hpp"

namespace CommandMove {
extern const ICommand *cmd;
}

static std::list<IObject *> Objects;

#undef main
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

  Scroller scroller{};

  while (true) {
    for (auto &obj : Objects) {
      obj->execute();
    }
    renderer.Render(scroller.GetPos(), Objects);
    scroller.execute();
  }

  return 0;
}
