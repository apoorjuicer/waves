#include "Audio.h"
#include <iostream>
#include <unordered_map>

// Initialize static audio storage
std::unordered_map<std::string, Audio> AudioManager::audioFiles;

void AudioManager::LoadAudio(const std::string& name, const std::string& filepath, bool isMusic, AEAudioGroup group) {
    if (audioFiles.find(name) != audioFiles.end()) {
        //std::cout << "Audio already loaded: " << name << std::endl;
        return; // Prevent duplicate loading
    }

    // If no valid group is provided, create a dedicated group.
    if (!AEAudioIsValidGroup(group)) {
        group = AEAudioCreateGroup();
        if (!AEAudioIsValidGroup(group)) {
            //std::cerr << "Failed to create audio group for: " << name << std::endl;
            return;
        }
    }

    AEAudio sound = isMusic ? AEAudioLoadMusic(filepath.c_str()) : AEAudioLoadSound(filepath.c_str());
    if (!AEAudioIsValidAudio(sound)) {
        //std::cerr << "Failed to load audio: " << filepath << std::endl;
        return;
    }

    audioFiles[name] = { sound, group, false };
    //std::cout << "Loaded audio: " << name << std::endl;
}

void AudioManager::UnloadAudio(const std::string& name) {
    auto it = audioFiles.find(name);
    if (it != audioFiles.end()) {
        AEAudioUnloadAudio(it->second.sound);
        AEAudioUnloadAudioGroup(it->second.group);
        audioFiles.erase(it);
        //std::cout << "Unloaded audio: " << name << std::endl;
    }
    else {
        //std::cout << "Audio not found: " << name << std::endl;
    }
}

void AudioManager::PlayAudio(const std::string& name, float volume, float pitch, int loops) {
    auto it = audioFiles.find(name);
    if (it != audioFiles.end()) {
        AEAudioPlay(it->second.sound, it->second.group, volume, pitch, loops);
        it->second.isPlaying = true;
        //std::cout << "Playing audio: " << name << std::endl;
    }
    else {
        //std::cerr << "Audio not found: " << name << std::endl;
    }
}

void AudioManager::StopAudio(const std::string& name) {
    auto it = audioFiles.find(name);
    if (it != audioFiles.end()) {
        AEAudioStopGroup(it->second.group);
        it->second.isPlaying = false;
        //std::cout << "Stopped audio: " << name << std::endl;
    }
    else {
        //std::cerr << "Audio not found: " << name << std::endl;
    }
}

bool AudioManager::IsAudioPlaying(const std::string& name) {
    auto it = audioFiles.find(name);
    if (it != audioFiles.end()) {
        return it->second.isPlaying;
    }
    return false;
}

void AudioManager::SetVolume(const std::string& name, float volume) {
    auto it = audioFiles.find(name);
    if (it != audioFiles.end()) {
        AEAudioSetGroupVolume(it->second.group, volume);
        //std::cout << "Set volume for " << name << " to " << volume << std::endl;
    }
    else {
        //std::cerr << "Audio not found: " << name << std::endl;
    }
}

void AudioManager::ShutdownAllAudio() {
    // Stop and unload every loaded audio file.
    for (auto it = audioFiles.begin(); it != audioFiles.end(); ) {
        // Stop audio before unloading.
        AEAudioStopGroup(it->second.group);
        AEAudioUnloadAudio(it->second.sound);
        AEAudioUnloadAudioGroup(it->second.group);
        //std::cout << "Shutting down audio: " << it->first << std::endl;
        it = audioFiles.erase(it);
    }
}