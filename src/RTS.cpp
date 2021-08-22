#include <SDL.h>
#include <SDL_image.h>
#include <SDL_timer.h>
#include <cstdio>
#undef main

#include "CommandMove.hpp"
#include "TestObject.hpp"
#include "VideoContext.hpp"

static SDL_Renderer *rend = nullptr;

int main(int, char **) {

  TestObject obj{Coord{0, 0, 0}};

  TestObject obj2{Coord{100, 300, 300}};

  CommandMove cmd{Coord{400, 400, 0}};

  obj.acceptCommand(cmd);
  obj2.acceptCommand(cmd);

  // retutns zero on success else non-zero
  if (SDL_Init(SDL_INIT_VIDEO) != 0) {
    printf("error initializing SDL: %s\n", SDL_GetError());
  }
  SDL_Window *win =
      SDL_CreateWindow("GAME", // creates a window
                       SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 1920,
                       1080, SDL_WINDOW_INPUT_GRABBED);

  // SDL_SetWindowFullscreen(win, true);
  SDL_ShowCursor(true);

  // triggers the program that controls
  // your graphics hardware and sets flags
  Uint32 render_flags = SDL_RENDERER_ACCELERATED;

  // creates a renderer to render our images
  rend = SDL_CreateRenderer(win, -1, render_flags);

  VideoContext::Create(rend);

  bool close = false;

  // annimation loop
  while (!close) {

    // Get the next event
    SDL_Event event;
    if (SDL_PollEvent(&event)) {
      if (event.type == SDL_QUIT) {
        break;
      }
    }

    // clears the screen
    SDL_RenderClear(rend);

    obj.execute();
    obj2.execute();

    // triggers the double buffers
    // for multiple rendering
    SDL_RenderPresent(rend);

    // calculates to 60 fps
    SDL_Delay(1000 / 60);
  }

  // destroy renderer
  SDL_DestroyRenderer(rend);

  // destroy window
  SDL_DestroyWindow(win);

  // close SDL
  SDL_Quit();

  return 0;
}
