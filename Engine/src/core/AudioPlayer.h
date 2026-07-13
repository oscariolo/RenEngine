#pragma once
#include <memory>

struct AudioPlayerImpl;

class AudioPlayer {
public:
    AudioPlayer(unsigned int sfxPoolSize = 8);
    ~AudioPlayer();

    void playSound(const char* filePath);

    void playMusic(const char* filePath, bool loop = true);
    void stopMusic();

    // Volume controls — 0.0 to 1.0
    void setSFXVolume(float volume);
    void setMusicVolume(float volume);

private:
    std::unique_ptr<AudioPlayerImpl> m_impl;
};