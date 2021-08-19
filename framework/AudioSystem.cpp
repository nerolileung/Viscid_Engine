#include "AudioSystem.h"

std::vector<const char*> AudioSystem::musicKeys;
std::map<const char*, Mix_Music*> AudioSystem::musicFiles;
int AudioSystem::musicKeyHead = 0;
std::vector<const char*> AudioSystem::clipKeys;
std::map<const char*, Mix_Chunk*> AudioSystem::clipFiles;
int AudioSystem::clipKeyHead = 0;

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

    // not really necessary
    musicKeys.clear();
    clipKeys.clear();
}

int AudioSystem::LoadMusic(const char* filename){
    // only load a file once
    if (musicFiles.count(filename) == 0){
        Mix_Music* music = Mix_LoadMUS(filename);

        // did it load succesfully?
        if (music != NULL){
            musicFiles[filename] = music;

            // add key at head
            std::vector<const char*>::iterator it = musicKeys.begin() + musicKeyHead;
            musicKeys.insert(it, filename);

            // update head
            for (int i = musicKeyHead; i <= musicKeys.size(); i++){
                if (i == musicKeys.size()){
                    musicKeyHead = i;
                    break;
                }
                // separate check to avoid invalid index
                else if (std::strcmp(musicKeys[i], "") == 0){
                    musicKeyHead = i;
                    break;
                }
            }

            // return id
            return musicKeys.size() - 1;
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

            // add key, update head
            std::vector<const char*>::iterator it = clipKeys.begin() + clipKeyHead;
            clipKeys.insert(it, filename);
            for (int i = clipKeyHead; i <= clipKeys.size(); i++){
                if (i == clipKeys.size()){
                    clipKeyHead = i;
                    break;
                }
                // separate check to avoid invalid index
                
                else if (std::strcmp(clipKeys[i], "") == 0){
                    clipKeyHead = i;
                    break;
                }
            }
            
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

void AudioSystem::UnloadMusic(const char* filename){
    // validate input
    if (musicFiles.count(filename) > 0){
        // remove file from map
        musicFiles.erase(filename);
        // remove key from vector
        for (int i = 0; i < musicKeys.size(); i++){
            if (musicKeys[i] == filename){
                musicKeys[i] = "";
                // update head index
                if (i < musicKeyHead)
                    musicKeyHead = i;
            }
        }
    }
}

void AudioSystem::UnloadMusic(int id){
    // validate
    if (id > 0 && id < musicKeys.size() && musicFiles.count(musicKeys[id]) > 0) {
        musicFiles.erase(musicKeys[id]);
        // no need to find index of key
        musicKeys[id] = "";
        if (id < musicKeyHead) musicKeyHead = id;
    }
}

void AudioSystem::UnloadClip(const char* filename){
    // validate input
    if (clipFiles.count(filename) > 0){
        clipFiles.erase(filename);
        // find and remove key
        for (int i = 0; i < clipKeys.size(); i++){
            if (clipKeys[i] == filename){
                clipKeys[i] = "";
                // update head index
                if (i < clipKeyHead)
                    clipKeyHead = i;
            }
        }
    }
}

void AudioSystem::UnloadClip(int id){
    // validate
    if (id > 0 && id < clipKeys.size() && clipFiles.count(clipKeys[id]) > 0) {
        clipFiles.erase(clipKeys[id]);
        // remove key, update head
        clipKeys[id] = "";
        if (id < clipKeyHead) clipKeyHead = id;
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
    // validate id
    if (id < 0 || id >= musicKeys.size())
        return false;
    // call filename overload
    else return PlayMusic(musicKeys[id]);
}

// default is 0 loops, -1 will be infinite loops
bool AudioSystem::PlayClip(const char* filename, int loops, int channel){
    // validate filename
    if (clipFiles.count(filename) == 0)
        return false;

    Mix_PlayChannel(channel, clipFiles[filename], loops);
    return true;
}

bool AudioSystem::PlayClip(int id, int loops, int channel){
    // validate id
    if (id < 0 || id >= clipKeys.size())
        return false;
    else return PlayClip(clipKeys[id], loops, channel);
}

bool AudioSystem::PlayClipFade(const char* filename, int loops, int channel){
    // validate filename
    if (clipFiles.count(filename) == 0)
        return false;

    Mix_FadeInChannel(channel, clipFiles[filename], loops, 1000);
    return true;
}

bool AudioSystem::PlayClipFade(int id, int loops, int channel){
    // validate id
    if (id < 0 || id >= clipKeys.size())
        return false;
    else return PlayClipFade(clipKeys[id], loops, channel);
}

void AudioSystem::StopMusic(){
    Mix_FadeOutMusic(100);
}

void AudioSystem::SetVolumeMusic(int newVolume){
    Mix_VolumeMusic(newVolume);
    volumeMusicCurrent = newVolume;
}

void AudioSystem::SetVolumeClips(int newVolume, int channel){
    Mix_Volume(channel, newVolume);
    volumeSFXCurrent = newVolume;
}

void AudioSystem::SetVolumeMaster(int newVolume){
    SetVolumeMusic(newVolume);
    SetVolumeClips(newVolume);
    volumeMasterCurrent = newVolume;
}

int AudioSystem::GetVolumeSFX(int channel){
    if (channel == -1) return volumeSFXCurrent;
    else return Mix_Volume(channel, -1);
}

bool AudioSystem::IsMusicPlaying(){
    return (Mix_PlayingMusic() == 1 ||
        Mix_FadingMusic() == MIX_FADING_IN ||
        Mix_FadingMusic() == MIX_FADING_OUT);
}