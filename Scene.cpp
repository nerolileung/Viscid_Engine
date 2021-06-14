#include "Scene.h"

void Scene::SetFonts(std::vector<TTF_Font*> aFonts){
    gameFonts = aFonts;
}

std::vector<TTF_Font*> Scene::gameFonts;

const std::vector<SDL_Colour> Scene::gameFontColours = {
    {255, 255, 255, 255}
};