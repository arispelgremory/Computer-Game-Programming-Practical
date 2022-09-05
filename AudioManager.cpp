#pragma once
#include "AudioManager.h"
#include <iostream>
using namespace std;

void AudioManager::InitializeAudio() {
    result = FMOD::System_Create(&system);
    result = system->init(32, FMOD_INIT_NORMAL, extradriverdata);
}

void AudioManager::PlaySound1() {
    result = system->playSound(sound1, 0, false, &channel);
}

void AudioManager::PlaySoundTrack() {
    result = system->playSound(soundTrack1, 0, false, &channel);
    channel->setPan(0); // -1 to 1
    channel->setVolume(0.5);
    channel->setPitch(1);
}

void AudioManager::LoadSounds(){
    result = system->createSound("Assets/Explosion-SoundEffect.mp3", FMOD_DEFAULT, 0, &sound1);
    if (result != FMOD_OK) {
        cout << "Load sound 1 failed!" << endl;
    }
    result = sound1->setMode(FMOD_LOOP_OFF);

    result = system->createStream("Assets/audio1.mp3", FMOD_DEFAULT, 0, &soundTrack1);
    if (result != FMOD_OK) {
        cout << "Load sound 2 failed!" << endl;
    }
    result = sound1->setMode(FMOD_LOOP_OFF);
}

void AudioManager::UpdateSound(){
    result = system->update();
}