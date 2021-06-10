#include "ViscidConfig.h"
#include "framework/include/SDL2/SDL.h"
#include "framework/include/SDL2/SDL_image.h"
#include <iostream>
#include <vector>
#include "framework/Game.h"
#include "framework/Logo.h"
#include "HatQuest.h"

int main(int argc, char *argv[])
{
  if (SDL_Init(SDL_INIT_VIDEO|SDL_INIT_AUDIO) < 0){
    std::cout << "Failed to initialise SDL!";
    return -1;
  }

  // "fullscreen" window
  SDL_Window *window = SDL_CreateWindow("HatQuest", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 640, 480, 0);
  int displayIndex = SDL_GetWindowDisplayIndex(window);
  SDL_Rect dimensions;
  SDL_GetDisplayBounds(displayIndex, &dimensions);
  SDL_SetWindowSize(window, dimensions.w, dimensions.h);
  Game::WindowHeight = dimensions.h;
  Game::WindowWidth = dimensions.w;

  // create renderer using first rendering driver with hardware acceleration
  SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
  if (!window){
    std::cout << "Failed to create window!";
    return -2;
  }

  // load extra file support from sdl_image and sdl_mixer
	IMG_Init(IMG_INIT_JPG | IMG_INIT_PNG);
  Mix_Init(MIX_INIT_MP3 | MIX_INIT_OGG);
  if (Mix_OpenAudio(MIX_DEFAULT_FREQUENCY, MIX_DEFAULT_FORMAT, 2, 1024) < 0) {
		std::cout << "Failed to initialise SDL_mixer! Error: " << Mix_GetError();
		return -3;
	}

  // create instance of specific game here
  HatQuest* game = new HatQuest();

  // logos to be displayed
  std::vector<Logo*> logos;
  logos.push_back(new Logo("data/logo.png",renderer,"data/logo.wav"));
  // add more logos here
  int logoIndex = 0;

  // put a null event in the queue
  SDL_Event event;

  // start measuring time
  float lastTime = (float)SDL_GetTicks() * 0.001f;

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
      if (!game->Update(deltaTime) || event.type == SDL_EventType::SDL_QUIT)
        break;

      // render loop; start by clearing to black
      SDL_SetRenderDrawColor(renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);
      SDL_RenderClear(renderer);
      game->Render(renderer);
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
  delete game;
  game = nullptr;

  // cleanup sdl
  SDL_DestroyWindow(window);
  Mix_Quit();
  IMG_Quit();
  SDL_Quit();

  return 0;
}