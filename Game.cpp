#include <chrono>
#include <vector>

#include "Config.hpp"
#include "GameLoop.hpp"
#include "RenderData.hpp"
#include "Renderer2D.hpp"
#include "Scroller.hpp"
#include "VideoContextSDL.hpp"

extern void addTestData(std::vector<IObject *> &Objects);

#undef main
int main(int, char **) {
  static std::vector<IObject *> Objects;
  addTestData(Objects);

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
