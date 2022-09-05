#pragma once
#include "fmod.hpp"

class AudioManager {
    public:
        FMOD::System* system;
        FMOD::Sound* sound1, * soundTrack1;
        FMOD::Channel* channel = 0;
        FMOD_RESULT    result;
        void* extradriverdata;

        void InitializeAudio();
        void PlaySound1();
        void PlaySoundTrack();
        void LoadSounds();
        void UpdateSound();

        //AudioManager();
        //~AudioManager();

};

