#ifndef LOGO_H
#define LOGO_H
#include "include/SDL2/SDL.h"
#include "include/SDL2/SDL_mixer.h"

class Logo
{
public:
    Logo(const char* imagePath, SDL_Renderer* aRenderer, const char* sfxPath = nullptr, float duration = 3.f, float sfxOffset = 1.f);
    ~Logo();
    void Update(float deltaTime);
    void Render(SDL_Renderer* aRenderer, SDL_Window* aWindow);
    bool IsFinished() { return finished; };
private:
    bool finished;
    SDL_Texture* image;
    float timerFreeze;
    float timerFadeIn;
    float timerFadeOut;
    int sfx;
    float sfxTimestamp;
    bool sfxPlayed;
};

#endif // LOGO_H