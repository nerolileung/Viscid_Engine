#include "ViscidConfig.h"
#include "framework/include/SDL2/SDL.h"
#include "Demo.h"
#include <iostream>

int main(int argc, char *argv[])
{
  // initialise sdl for graphics
  if (SDL_Init(SDL_INIT_VIDEO) < 0){
    std::cout << "Failed to initialise video!";
    exit(-1);
  }

  // create 640x480 window
  SDL_Window *window = SDL_CreateWindow("Viscid Framework Demo", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 640, 480, 0);
  // create renderer with window context using first rendering driver with hardware acceleration
  SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
  if (!window){
    std::cout << "Failed to create window!";
    exit(-1);
  }

  // create game instance
  Demo* demo = new Demo();

  // put a null event in the queue
  SDL_Event event;

  // start measuring time
  float lastTime = (float)SDL_GetTicks() * 0.001f;

  // game loop; event is always 1 and polling it updates the keyboard state
  while (SDL_PollEvent(&event) >= 0){
    // update time
    float currentTime = (float)SDL_GetTicks() * 0.001f;
    float deltaTime = currentTime - lastTime;

    // update loop
    if (!demo->Update(deltaTime) || event.type == SDL_EventType::SDL_QUIT)
      break;

    // render loop; start by clearing to black
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);
    SDL_RenderClear(renderer);
    demo->Render(renderer);
    SDL_RenderPresent(renderer);

    lastTime = currentTime;
    SDL_Delay(1);
  }

  // cleanup game
  delete demo;
  demo = nullptr;

  // cleanup sdl
  SDL_DestroyWindow(window);
  SDL_Quit();

  return 0;
}