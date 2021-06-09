#ifndef LOGO_H
#define LOGO_H
#include "include/SDL2/SDL.h"

class Logo
{
public:
    Logo(const char* filepath, SDL_Renderer* aRenderer, float duration = 3.f);
    ~Logo();
    void Update(float deltaTime);
    void Render(SDL_Renderer* aRenderer);
    bool IsFinished() { return finished; };
private:
    bool finished;
    SDL_Texture* image;
    float timerFreeze;
    float timerFadeIn;
    float timerFadeOut;
};

#endif // LOGO_H