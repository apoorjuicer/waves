#include "Audio.h"

// Initialize static audio storage
std::unordered_map<std::string, Audio> AudioManager::audioFiles;
f32 AudioManager::masterVolume = 1.0f;

void AudioManager::LoadAudio(const std::string& name, const std::string& filepath, bool isMusic, AEAudioGroup group) {
    if (audioFiles.find(name) != audioFiles.end())
        return;

    if (!AEAudioIsValidGroup(group)) {
        group = AEAudioCreateGroup();
        if (!AEAudioIsValidGroup(group))
            return;
    }

    AEAudio sound = isMusic ? AEAudioLoadMusic(filepath.c_str()) : AEAudioLoadSound(filepath.c_str());
    if (!AEAudioIsValidAudio(sound))
        return;

    // Immediately apply master volume to the group
    AEAudioSetGroupVolume(group, masterVolume);

    audioFiles[name] = { sound, group, false };
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

// Master volume functions
void AudioManager::SetMasterVolume(float volume) {
    masterVolume = volume;
    //std::cout << "Master volume set to: " << masterVolume << std::endl;
    // Optionally update all currently playing audio groups
    UpdateAllVolumes();
}

float AudioManager::GetMasterVolume() {
    return masterVolume;
}

void AudioManager::UpdateAllVolumes() {
    for (auto& kv : audioFiles) {
        AEAudioSetGroupVolume(kv.second.group, masterVolume);
    }
}
