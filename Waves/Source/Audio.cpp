#include "Audio.h"
#include <iostream>

// Initialize static audio storage
std::unordered_map<std::string, Audio> AudioManager::audioFiles;

void AudioManager::LoadAudio(const std::string& name, const std::string& filepath, bool isMusic, AEAudioGroup group) {
    if (audioFiles.find(name) != audioFiles.end()) return;

    AEAudio sound = isMusic ? AEAudioLoadMusic(filepath.c_str()) : AEAudioLoadSound(filepath.c_str());

    audioFiles[name] = { sound, group };  // Use an existing group instead of creating a new one
}

void AudioManager::UnloadAudio(const std::string& name) {
    auto it = audioFiles.find(name);
    if (it != audioFiles.end()) {
        AEAudioUnloadAudio(it->second.sound);
        audioFiles.erase(it);
        std::cout << "Unloaded audio: " << name << std::endl;
    }
}

void AudioManager::PlayAudio(const std::string& name, float volume, float pitch, int loops) {
    auto it = audioFiles.find(name);
    if (it != audioFiles.end()) {
        AEAudioPlay(it->second.sound, it->second.group, volume, pitch, loops);
        std::cout << "Playing audio: " << name << " at volume " << volume << std::endl;
    }
}

void AudioManager::SetVolume(const std::string& name, float volume) {
    auto it = audioFiles.find(name);
    if (it != audioFiles.end()) {
        AEAudioSetGroupVolume(it->second.group, volume);
        std::cout << "Set volume for " << name << " to " << volume << std::endl;
    }
}


