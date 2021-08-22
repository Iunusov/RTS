#include <SDL.h>
#include <SDL_image.h>
#include <SDL_timer.h>
#include <cstdio>
#undef main

#include "CommandMove.hpp"
#include "TestObject.hpp"

int main(int, char **) {

  TestObject obj{Coord{0, 0, 0}};
  CommandMove cmd{Coord{400, 400, 0}};

  obj.acceptCommand(cmd);

  // retutns zero on success else non-zero
  if (SDL_Init(SDL_INIT_VIDEO) != 0) {
    printf("error initializing SDL: %s\n", SDL_GetError());
  }
  SDL_Window *win = SDL_CreateWindow("GAME", // creates a window
                                     SDL_WINDOWPOS_CENTERED,
                                     SDL_WINDOWPOS_CENTERED, 1000, 1000, 0);

  // triggers the program that controls
  // your graphics hardware and sets flags
  Uint32 render_flags = SDL_RENDERER_ACCELERATED;

  // creates a renderer to render our images
  SDL_Renderer *rend = SDL_CreateRenderer(win, -1, render_flags);

  // creates a surface to load an image into the main memory
  SDL_Surface *surface;

  // please provide a path for your image
  surface = SDL_LoadBMP("assets/test.bmp");
  if (!surface) {
    return 9;
  }

  // loads image to our graphics hardware memory.
  SDL_Texture *tex = SDL_CreateTextureFromSurface(rend, surface);

  // clears main-memory
  SDL_FreeSurface(surface);

  // let us control our image position
  // so that we can move it with our keyboard.
  SDL_Rect dest;

  // connects our texture with dest to control position
  SDL_QueryTexture(tex, NULL, NULL, &dest.w, &dest.h);

  // adjust height and width of our image box.
  dest.w /= 6;
  dest.h /= 6;

  // sets initial x-position of object
  dest.x = (int)obj.getPosition().x;

  // sets initial y-position of object
  dest.y = (int)obj.getPosition().y;

  // controls annimation loop
  int close = 0;

  // annimation loop
  while (!close) {

    SDL_Event event;

    // Events management
    while (SDL_PollEvent(&event)) {
    }
    dest.x = (int)obj.getPosition().x;

    dest.y = (int)obj.getPosition().y;

    // clears the screen
    SDL_RenderClear(rend);
    SDL_RenderCopy(rend, tex, NULL, &dest);

    // triggers the double buffers
    // for multiple rendering
    SDL_RenderPresent(rend);

    // calculates to 60 fps
    SDL_Delay(1000 / 60);

    obj.execute();
  }

  // destroy texture
  SDL_DestroyTexture(tex);

  // destroy renderer
  SDL_DestroyRenderer(rend);

  // destroy window
  SDL_DestroyWindow(win);

  // close SDL
  SDL_Quit();

  return 0;
}
