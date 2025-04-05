/* Start Header ************************************************************************/
/*!
\file		audio.h
\author		Jordain
\co-author	Henrey Hu
\email		jordainyingzhe.ng@digipen.edu

\brief      This file defines the `AudioManager` class and related structures for managing audio in the game.
            It provides functionality for loading, playing, stopping, and managing audio files and groups.

Copyright (C) 2025 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents
without the prior written consent of DigiPen Institute of
Technology is prohibited.
*/
/* End Header **************************************************************************/

#ifndef AUDIO_MANAGER_H
#define AUDIO_MANAGER_H

#include "pch.h"

// ========================================
// Structure to hold audio information
// ========================================
struct Audio {
    AEAudio sound = {};         // The actual audio object
	AEAudioGroup group = {};    // The group to which the audio belongs
    bool isPlaying = false;     // Track whether the audio is playing
};

// ========================================
// AudioManager class for managing audio
// ========================================
class AudioManager {
public:
	// Load audio files into the manager
    static void LoadAudio(const std::string& name, const std::string& filepath, bool isMusic, AEAudioGroup group = {});

	// Unload audio files from the manager
    static void UnloadAudio(const std::string& name);

	// Play audio files
    static void PlayAudio(const std::string& name, float volume = 1.0f, float pitch = 1.0f, int loops = -1);

	// Stop audio files
    static void StopAudio(const std::string& name);

	// Pause audio files
    static bool IsAudioPlaying(const std::string& name);

	// Check if audio is loaded
    static void SetVolume(const std::string& name, float volume);

	// Get the volume of audio
    static void ShutdownAllAudio();

	// Get the volume of audio
    static void SetMasterVolume(float volume);

	// Get the master volume
    static float GetMasterVolume();

	// Get the volume of audio
    static void UpdateAllVolumes();

private:
	// Map to hold audio files
    static std::unordered_map<std::string, Audio> audioFiles;

	// Map to hold audio groups
    static f32 masterVolume;
};

#endif // AUDIO_MANAGER_H
