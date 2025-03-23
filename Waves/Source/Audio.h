#ifndef AUDIO_MANAGER_H
#define AUDIO_MANAGER_H

#include "AEEngine.h"
#include <unordered_map>
#include <string>
#include <iostream>

struct Audio {
    AEAudio sound = {};
    AEAudioGroup group = {};
    bool isPlaying = false;  // ✅ Track whether the audio is playing
};

class AudioManager {
public:
    static void LoadAudio(const std::string& name, const std::string& filepath, bool isMusic, AEAudioGroup group);
    static void UnloadAudio(const std::string& name);
    static void PlayAudio(const std::string& name, float volume = 1.0f, float pitch = 1.0f, int loops = -1);
    static void StopAudio(const std::string& name);
    static bool IsAudioPlaying(const std::string& name);
    static void SetVolume(const std::string& name, float volume);

private:
    static std::unordered_map<std::string, Audio> audioFiles;
};

#endif // AUDIO_MANAGER_H
