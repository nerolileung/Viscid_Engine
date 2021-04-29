#include "ViscidConfig.h"
#include "SDL2/SDL.h"
#include <iostream>

int main(int argc, char *argv[])
{
  // initialise sdl for graphics
  if (SDL_Init(SDL_INIT_VIDEO) < 0){
    std::cout << "Failed to initialise video!";
    exit(-1);
  }

  // create 640x480 window
  SDL_Window *window = SDL_CreateWindow("SDL2Test", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 640, 480, 0);
  // create renderer with window context using first rendering driver with hardware acceleration
  SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
  if (!window){
    std::cout << "Failed to create window!";
    exit(-1);
  }

  // put a null event in the queue
  SDL_Event event;
  // game loop; event is always 1 and polling it updates the keyboard state
  while (SDL_PollEvent(&event) >= 0){
    // update loop; check if escape has been pressed
    const Uint8* keystate = SDL_GetKeyboardState(NULL);
    if (keystate[SDL_SCANCODE_ESCAPE])
      break;

    // render loop; clear to black
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);
    SDL_RenderClear(renderer);
    SDL_RenderPresent(renderer);
  }

  SDL_DestroyWindow(window);
  SDL_Quit();

  return 0;
}