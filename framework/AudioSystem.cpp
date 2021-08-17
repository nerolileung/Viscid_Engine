#include "AudioSystem.h"

std::vector<const char*> AudioSystem::musicKeys;
std::map<const char*, Mix_Music*> AudioSystem::musicFiles;
std::vector<const char*> AudioSystem::clipKeys;
std::map<const char*, Mix_Chunk*> AudioSystem::clipFiles;

int AudioSystem::volumeMasterCurrent = MIX_MAX_VOLUME;
//int AudioSystem::volumeMasterPrevious = MIX_MAX_VOLUME;
int AudioSystem::volumeMusicCurrent = MIX_MAX_VOLUME;
int AudioSystem::volumeSFXCurrent = MIX_MAX_VOLUME;

AudioSystem::~AudioSystem(){
    std::map<const char*, Mix_Music*>::iterator itMusic;
    for (itMusic = musicFiles.begin(); itMusic != musicFiles.end(); itMusic++){
        Mix_FreeMusic(itMusic->second);
    }

    std::map<const char*, Mix_Chunk*>::iterator itClips;
    for (itClips = clipFiles.begin(); itClips != clipFiles.end(); itClips++){
        Mix_FreeChunk(itClips->second);
    }
}

int AudioSystem::LoadMusic(const char* filename){
    // only load a file once
    if (musicFiles.count(filename) == 0){
        Mix_Music* music = Mix_LoadMUS(filename);
        // did it load succesfully?
        if (music != NULL){
            musicFiles[filename] = music;
            musicKeys.push_back(filename);
            return musicKeys.size() - 1; // return id
        }
        else return -1;
    }
    else {
        // return existing id
        std::vector<const char*>::iterator it = std::find(musicKeys.begin(), musicKeys.end(), filename);
        return it - musicKeys.begin();
    }
}

int AudioSystem::LoadClip(const char* filename){
    // only load a file once
    if (clipFiles.count(filename) == 0){
        Mix_Chunk* clip = Mix_LoadWAV(filename);
        // did it load succesfully?
        if (clip != NULL){
            clipFiles[filename] = clip;
            clipKeys.push_back(filename);
             // return id
            return clipKeys.size() - 1;
        }
        else return -1;
    }
    else {
        // return existing id
        std::vector<const char*>::iterator it = std::find(clipKeys.begin(), clipKeys.end(), filename);
        return it - clipKeys.begin();
    }
}

bool AudioSystem::PlayMusic(const char* filename){
    // check if this is a file we have
    if (musicFiles.count(filename) == 0)
        return false; // todo attempt to load file?

    if (Mix_PlayingMusic() == 0){
        // todo account for music paused?
        StopMusic();
    }
    Mix_FadeInMusic(musicFiles[filename], -1, 1000);
    return true;
}

bool AudioSystem::PlayMusic(int id){
    // validate id and filename
    if (id < 0 || id >= musicKeys.size() ||
        musicFiles.count(musicKeys[id]) == 0)
        return false;

    if (Mix_PlayingMusic() == 0)
        StopMusic();
    
    Mix_FadeInMusic(musicFiles[musicKeys[id]], -1, 1000);
    return true;
}

// default is 0 loops, -1 will be infinite loops
bool AudioSystem::PlayClip(const char* filename, int loops = 0, int channel = -1){
    // validate filename
    if (clipFiles.count(filename) == 0)
        return false;

    Mix_PlayChannel(channel, clipFiles[filename], loops);
    return true;
}

bool AudioSystem::PlayClip(int id, int loops = 0, int channel = -1){
    // validate id and filename
    if (id < 0 || id >= clipKeys.size() ||
        clipFiles.count(clipKeys[id]) == 0)
        return false;

    Mix_PlayChannel(channel, clipFiles[clipKeys[id]], loops);
    return true;
}

bool AudioSystem::PlayClipFade(const char* filename, int loops = 0, int channel = -1){
    // validate filename
    if (clipFiles.count(filename) == 0)
        return false;

    Mix_FadeInChannel(channel, clipFiles[filename], loops, 1000);
    return true;
}

bool AudioSystem::PlayClipFade(int id, int loops = 0, int channel = -1){
    // validate id and filename
    if (id < 0 || id >= clipKeys.size() ||
        clipFiles.count(clipKeys[id]) == 0)
        return false;

    Mix_FadeInChannel(channel, clipFiles[clipKeys[id]], loops, 1000);
    return true;
}

void AudioSystem::StopMusic(){
    Mix_FadeOutMusic(100);
}

void AudioSystem::SetVolumeMusic(int newVolume){
    Mix_VolumeMusic(newVolume);
    volumeMusicCurrent = newVolume;
}

void AudioSystem::SetVolumeClips(int newVolume, int channel = -1){
    Mix_Volume(channel, newVolume);
    volumeSFXCurrent = newVolume;
}

void AudioSystem::SetVolumeMaster(int newVolume){
    SetVolumeMusic(newVolume);
    SetVolumeClips(newVolume);
    volumeMasterCurrent = newVolume;
}

int AudioSystem::GetVolumeSFX(int channel = -1){
    if (channel == -1) return volumeSFXCurrent;
    else return Mix_Volume(channel, -1);
}

bool AudioSystem::IsMusicPlaying(){
    return (Mix_PlayingMusic() == 1 ||
        Mix_FadingMusic() == MIX_FADING_IN ||
        Mix_FadingMusic() == MIX_FADING_OUT);
}