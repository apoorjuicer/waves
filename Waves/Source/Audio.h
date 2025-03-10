#pragma once
#include "AEEngine.h"
#include <string>
#include <unordered_map>

// Struct to store audio data
struct Audio {
    AEAudio sound;
    AEAudioGroup group;
};

// Audio Manager
class AudioManager {
public:
    // Load and Unload Audio
    static void LoadAudio(const std::string& name, const std::string& filepath, bool isMusic, AEAudioGroup group);
    static void UnloadAudio(const std::string& name);

    // Play Audio
    static void PlayAudio(const std::string& name, float volume = 1.0f, float pitch = 1.0f, int loops = 0);
    static void SetVolume(const std::string& name, float volume);


private:
    static std::unordered_map<std::string, Audio> audioFiles;
};
