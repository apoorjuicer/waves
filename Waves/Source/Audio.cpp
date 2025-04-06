/* Start Header ************************************************************************/
/*!
\file		Audio.cpp
\project    Waves

\author		Jordain
\email		jordainyingzhe.ng@digipen.edu
\co-author	Henrey Hu(%15)

\brief      This file defines the `AudioManager` class and related structures for managing audio in the game.
            It provides functionality for loading, playing, stopping, and managing audio files and groups.

Copyright (C) 2025 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents
without the prior written consent of DigiPen Institute of
Technology is prohibited.
*/
/* End Header **************************************************************************/

#include "Audio.h"

// Initialize static audio storage
std::unordered_map<std::string, Audio> AudioManager::audioFiles;
// Initialize static master volume
f32 AudioManager::masterVolume = 1.0f;

// Initialize static audio group
void AudioManager::LoadAudio(const std::string& name, const std::string& filepath, bool isMusic, AEAudioGroup group) {
	// Check if the audio file is already loaded
    if (audioFiles.find(name) != audioFiles.end())
        return;

	// Check if the file exists
    if (!AEAudioIsValidGroup(group)) {
        group = AEAudioCreateGroup();
        if (!AEAudioIsValidGroup(group))
            return;
    }

	// Load the audio file
    AEAudio sound = isMusic ? AEAudioLoadMusic(filepath.c_str()) : AEAudioLoadSound(filepath.c_str());
    if (!AEAudioIsValidAudio(sound))
        return;

    // Immediately apply master volume to the group
    AEAudioSetGroupVolume(group, masterVolume);

	// Set the audio group to the loaded audio
    audioFiles[name] = { sound, group, false };
}

// Unload the audio file and group
void AudioManager::UnloadAudio(const std::string& name) {
	// Check if the audio file is loaded
    auto it = audioFiles.find(name);

	// If found, unload the audio and group
    if (it != audioFiles.end()) {
        AEAudioUnloadAudio(it->second.sound);
        AEAudioUnloadAudioGroup(it->second.group);
        audioFiles.erase(it);
    }
}

// Play the audio file
void AudioManager::PlayAudio(const std::string& name, float volume, float pitch, int loops) {
	// Check if the audio file is loaded
    auto it = audioFiles.find(name);

	// If found, play the audio
    if (it != audioFiles.end()) {
        AEAudioPlay(it->second.sound, it->second.group, volume, pitch, loops);
        it->second.isPlaying = true;
    }
}

// Stop the audio file
void AudioManager::StopAudio(const std::string& name) {
	// Check if the audio file is loaded
    auto it = audioFiles.find(name);

	// If found, stop the audio
    if (it != audioFiles.end()) {
        AEAudioStopGroup(it->second.group);
        it->second.isPlaying = false;
    }
}

// Pause the audio file
bool AudioManager::IsAudioPlaying(const std::string& name) {
	// Check if the audio file is loaded
    auto it = audioFiles.find(name);

	// If found, return the playing status
    if (it != audioFiles.end()) {
        return it->second.isPlaying;
    }
    return false;
}

// Check if the audio file is loaded
void AudioManager::SetVolume(const std::string& name, float volume) {
	// Check if the audio file is loaded
    auto it = audioFiles.find(name);

	// If found, set the volume
    if (it != audioFiles.end()) {
        AEAudioSetGroupVolume(it->second.group, volume);
    }
}

// Check if the audio file is loaded
void AudioManager::ShutdownAllAudio() {
    // Stop and unload every loaded audio file.
    for (auto it = audioFiles.begin(); it != audioFiles.end(); ) {
        // Stop audio before unloading.
        AEAudioStopGroup(it->second.group);
        AEAudioUnloadAudio(it->second.sound);
        AEAudioUnloadAudioGroup(it->second.group);
        it = audioFiles.erase(it);
    }
}

// Master volume functions
void AudioManager::SetMasterVolume(float volume) {
    masterVolume = volume;
    UpdateAllVolumes();
}

// Get the master volume
float AudioManager::GetMasterVolume() {
    return masterVolume;
}

// Update the volume of all audio files
void AudioManager::UpdateAllVolumes() {
	// Update the volume of all loaded audio files to match the master volume
    for (auto& kv : audioFiles) {
        AEAudioSetGroupVolume(kv.second.group, masterVolume);
    }
}
