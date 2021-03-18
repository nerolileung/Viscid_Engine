#include "ViscidConfig.h"
#include <SDL/SDL.h>

int main()
{
    if (InitSDL()){
        return 1;
    }
    return 0;
}

bool InitSDL() {
	//setup SDL
	if (SDL_Init(SDL_INIT_VIDEO) < 0) { //returns -1 on failure, 0 on success
		cout << "SDL did not initialise. Error: " << SDL_GetError();
		return false;
	}
	else {
		//attempt to create window
		gWindow = SDL_CreateWindow("Games Engine Creation", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
		if (gWindow == NULL) {
			cout << "Window was not created. Error: " << SDL_GetError();
			return false;
		}
		gRenderer = SDL_CreateRenderer(gWindow, -1, SDL_RENDERER_ACCELERATED); //use rendering driver that supports hardware acceleration
		if (gRenderer != NULL) {
			//initialise png loading
			int imageFlags = IMG_INIT_PNG;
			if (!(IMG_Init(imageFlags) & imageFlags)) { //why &
				cout << "SDL_Image initialisation failed. Error: " << IMG_GetError();
				return false;
			}
			gTexture = new Texture2D(gRenderer);
			if (!gTexture->LoadFromFile("Images/BackgroundMB.png")) {
				return false;
			}
			if (gTexture == NULL) return false;
			//initialise mixer
			if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 1024) < 0) {
				cout << "Mixer initialisation failed. Error: " << Mix_GetError();
				return false;
			}
		}
		else {
			cout << "Renderer could not initialise. Error: " << SDL_GetError();
			return false;
		}
		return true;
	}
}