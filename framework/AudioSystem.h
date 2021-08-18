#ifndef AUDIOSYSTEM_H
#define AUDIOSYSTEM_H

#include "include/SDL2/SDL.h"
#include "include/SDL2/SDL_mixer.h"
#include <vector>
#include <map>

class AudioSystem{
public:
    static int LoadMusic(const char* filename);
    static int LoadClip(const char* filename);

    static bool PlayMusic(const char* filename);
    static bool PlayMusic(int id); // in case we want to use enums

    static void StopMusic();

    static bool PlayClip(const char* filename, int loops = 0, int channel = -1);
    static bool PlayClip(int id, int loops = 0, int channel = -1);
    // set up bit flags for various fx if this gets bloated
    static bool PlayClipFade(const char* filename, int loops = 0, int channel = -1);
    static bool PlayClipFade(int id, int loops = 0, int channel = -1);

    static void SetVolumeMaster(int newVolume);
    static void SetVolumeMusic(int newVolume);
    static void SetVolumeClips(int newVolume, int channel = -1);

    static int GetVolumeMaster() { return volumeMasterCurrent; };
    static int GetVolumeMusic() { return volumeMusicCurrent; };
    static int GetVolumeSFX(int channel = -1);

    static bool IsMusicPlaying();

    // todo unload audio functions for saving space

private:
    static std::vector<const char*> musicKeys;
    static std::map<const char*, Mix_Music*> musicFiles;

    static std::vector<const char*> clipKeys;
    static std::map<const char*, Mix_Chunk*> clipFiles;

    static int volumeMasterCurrent;
    //static int volumeMasterPrevious;
    static int volumeMusicCurrent;
    static int volumeSFXCurrent;

    ~AudioSystem();
};

#endif // AUDIOSYSTEM_H