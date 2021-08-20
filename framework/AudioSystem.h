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

    static void UnloadMusic(const char* filename);
    static void UnloadMusic(int id);

    static void UnloadClip(const char* filename);
    static void UnloadClip(int id);

    static bool PlayMusic(const char* filename, int loops = -1);
    static bool PlayMusic(int id, int loops = -1); // int id in case we want to use enums
    static bool PlayMusicFade(const char* filename, int loops = -1);
    static bool PlayMusicFade(int id, int loops = -1);

    static bool IsMusicPlaying();
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

private:
    static std::vector<const char*> musicKeys;
    static std::map<const char*, Mix_Music*> musicFiles;
    static int musicKeyHead;

    static std::vector<const char*> clipKeys;
    static std::map<const char*, Mix_Chunk*> clipFiles;
    static int clipKeyHead;

    static int volumeMasterCurrent;
    //static int volumeMasterPrevious;
    static int volumeMusicCurrent;
    static int volumeSFXCurrent;

    ~AudioSystem();
};

#endif // AUDIOSYSTEM_H