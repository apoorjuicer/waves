#include "Audio.h"
#include <iostream>
#include <unordered_map>

// Initialize static audio storage
std::unordered_map<std::string, Audio> AudioManager::audioFiles;

void AudioManager::LoadAudio(const std::string& name, const std::string& filepath, bool isMusic, AEAudioGroup group) {
    if (audioFiles.find(name) != audioFiles.end()) return; // Prevent duplicate loading

    AEAudio sound = isMusic ? AEAudioLoadMusic(filepath.c_str()) : AEAudioLoadSound(filepath.c_str());

    if (!AEAudioIsValidAudio(sound)) {
        //std::cerr << "Failed to load audio: " << filepath << std::endl;
        return;
    }

    audioFiles[name] = { sound, group };
    //std::cout << "Loaded audio: " << name << std::endl;
}

void AudioManager::UnloadAudio(const std::string& name) {
    auto it = audioFiles.find(name);
    if (it != audioFiles.end()) {
        AEAudioUnloadAudio(it->second.sound);
        //audioFiles.erase(it);
        //std::cout << "Unloaded audio: " << name << std::endl;
    }
}

void AudioManager::PlayAudio(const std::string& name, float volume, float pitch, int loops) {
    auto it = audioFiles.find(name);
    if (it != audioFiles.end()) {
        //AEAudioPlay(it->second.sound, it->second.group, volume, pitch, loops);
        //std::cout << "Playing audio: " << name << " at volume " << volume << std::endl;
    }
}

void AudioManager::StopAudio(const std::string& name) {
    auto it = audioFiles.find(name);
    if (it != audioFiles.end()) {
        AEAudioStopGroup(it->second.group);
        //std::cout << "Stopped audio: " << name << std::endl;
    }
}

void AudioManager::SetVolume(const std::string& name, float volume) {
    auto it = audioFiles.find(name);
    if (it != audioFiles.end()) {
        AEAudioSetGroupVolume(it->second.group, volume);
        //std::cout << "Set volume for " << name << " to " << volume << std::endl;
    }
}

bool AudioManager::IsAudioPlaying(const std::string& name) {
    auto it = audioFiles.find(name);
    if (it != audioFiles.end()) {
        // Since AEAudioIsPlaying does not exist, we assume it's playing after being played
        return true;
    }
    return false;
}
