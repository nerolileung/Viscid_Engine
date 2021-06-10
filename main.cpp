#include "ViscidConfig.h"
#include "framework/include/SDL2/SDL.h"
#include "framework/include/SDL2/SDL_image.h"
#include <iostream>
#include <vector>
#include "framework/Logo.h"
#include "Demo.h"

int main(int argc, char *argv[])
{
  // initialise sdl for graphics
  if (SDL_Init(SDL_INIT_VIDEO|SDL_INIT_AUDIO) < 0){
    std::cout << "Failed to initialise SDL!";
    return -1;
  }

  // create 640x480 window
  SDL_Window *window = SDL_CreateWindow("Viscid Framework Demo", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 640, 480, 0);
  // create renderer with window context using first rendering driver with hardware acceleration
  SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
  if (!window){
    std::cout << "Failed to create window!";
    return -2;
  }

  // load support for .jpg and .png files
	IMG_Init(IMG_INIT_JPG | IMG_INIT_PNG);
  // load support for .mp3 and .ogg files
  Mix_Init(MIX_INIT_MP3 | MIX_INIT_OGG);
  if (Mix_OpenAudio(MIX_DEFAULT_FREQUENCY, MIX_DEFAULT_FORMAT, 2, 1024) < 0) {
		std::cout << "Failed to initialise SDL_mixer! Error: " << Mix_GetError();
		return -3;
	}

  // create game instance
  Demo* demo = new Demo();

  // put a null event in the queue
  SDL_Event event;

  // start measuring time
  float lastTime = (float)SDL_GetTicks() * 0.001f;

  // display logos
  std::vector<Logo*> logos;
  logos.push_back(new Logo("data/logo.png",renderer,"data/logo.wav"));
  int logoIndex = 0;

  // game loop; event is always 1 and polling it updates the keyboard state
  while (SDL_PollEvent(&event) >= 0){
    // update time
    float currentTime = (float)SDL_GetTicks() * 0.001f;
    float deltaTime = currentTime - lastTime;

    // display logos
    if (logoIndex < logos.size()){
      logos[logoIndex]->Update(deltaTime);
      SDL_SetRenderDrawColor(renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);
      SDL_RenderClear(renderer);
      logos[logoIndex]->Render(renderer);
      SDL_RenderPresent(renderer);
      if (logos[logoIndex]->IsFinished()) logoIndex++;
    }
    else {
      // update loop
      if (!demo->Update(deltaTime) || event.type == SDL_EventType::SDL_QUIT)
        break;

      // render loop; start by clearing to black
      SDL_SetRenderDrawColor(renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);
      SDL_RenderClear(renderer);
      demo->Render(renderer);
      SDL_RenderPresent(renderer);
    }

    lastTime = currentTime;
    SDL_Delay(1);
  }

  // cleanup logos
  for (int i = 0; i < logoIndex; i++){
    delete logos[i];
    logos[i] = nullptr;
  }

  // cleanup game
  delete demo;
  demo = nullptr;

  // cleanup sdl
  SDL_DestroyWindow(window);
  Mix_Quit();
  IMG_Quit();
  SDL_Quit();

  return 0;
}