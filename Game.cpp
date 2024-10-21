#include <chrono>
#include <thread>
#include <vector>
#include <iostream>

#include "Config.hpp"
#include "GameLoop.hpp"
#include "IObject.hpp"
#include "RenderData.hpp"
#include "Renderer2D.hpp"
#include "Scroller.hpp"
#include "VideoContextSDL.hpp"

#include <FL/Fl.H>
#include <FL/Fl_Button.H>
#include <FL/Fl_Single_Window.H>
#include <FL/Fl_Window.H>
#include <FL/x.H>

#undef main
int main(int, char **) {
  Fl::scheme("gtk+");

  int w;
  int h;
  SDL_Init(SDL_INIT_VIDEO);
  VideoContextSDL::getPrimaryDisplayResolution(w, h);
  SDL_Quit();
  
  std::cout<<w<<" "<<h<<std::endl;

  // Create main window
  auto window = new Fl_Window(w, h, "RTS");
  window->resizable(window);
  window->fullscreen();
  window->show();

  GameLoop game;

  // SDL rendering thread
  std::thread t([&game, window]() {
    VideoContextSDL::Create(fl_xid(window));
    VideoContextSDL::GetInstance()->setup();
    IRenderer *renderer = new Renderer2D{VideoContextSDL::GetInstance()};
    RenderData renderFrame{};

    game.Start(renderFrame, *renderer);

    while (true) {
      static Scroller scroller{};
      scroller.execute();

      const size_t expectedMS{
          (size_t)(1000.0 / VideoContextSDL::GetInstance()->getFps())};
      const auto start{std::chrono::steady_clock::now()};

      static std::chrono::time_point<std::chrono::steady_clock> timestamp{};
      static std::vector<IObject *> lastRender;
      renderFrame.GetRenderData(lastRender, timestamp);

      renderer->Render(scroller.GetCameraPos(), scroller.getScale(), lastRender,
                       timestamp);

      const auto end{std::chrono::steady_clock::now()};
      const auto elapsedMS{(size_t)(
          std::chrono::duration_cast<std::chrono::milliseconds>(end - start)
              .count())};

      if (expectedMS > elapsedMS) {
        renderer->Delay((size_t)(expectedMS - elapsedMS));
      }
    }
  });
  t.detach();

  return Fl::run();
}
