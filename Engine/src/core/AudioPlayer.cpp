#define MINIAUDIO_IMPLEMENTATION
#include <miniaudio.h>
#include "AudioPlayer.h"
#include <iostream>
#include <vector>

struct SFXSlot {
    ma_sound sound;
    bool     inUse = false;
};

struct AudioPlayerImpl {
    ma_engine           engine;
    std::vector<SFXSlot> sfxPool;
    int                 nextSlot = 0;   // round-robin index

    ma_sound            music;
    bool                musicLoaded = false;
};

// ---------------------------------------------------------------- lifecycle

AudioPlayer::AudioPlayer(unsigned int sfxPoolSize)
    : m_impl(std::make_unique<AudioPlayerImpl>())
{
    ma_result result = ma_engine_init(NULL, &m_impl->engine);
    if (result != MA_SUCCESS) {
        std::cerr << "AudioPlayer: failed to init engine\n";
        return;
    }
    m_impl->sfxPool.resize(sfxPoolSize);
}

AudioPlayer::~AudioPlayer() {
    for (auto& slot : m_impl->sfxPool) {
        if (slot.inUse) {
            ma_sound_stop(&slot.sound);
            ma_sound_uninit(&slot.sound);
        }
    }
    if (m_impl->musicLoaded) {
        ma_sound_stop(&m_impl->music);
        ma_sound_uninit(&m_impl->music);
    }
    ma_engine_uninit(&m_impl->engine);
}

// ---------------------------------------------------------------- SFX

void AudioPlayer::playSound(const char* filePath) {
    // Walk the pool from the current index to find a free slot.
    // If none are free, evict the oldest (nextSlot wraps around).
    const int poolSize = static_cast<int>(m_impl->sfxPool.size());
    int chosen = m_impl->nextSlot;

    for (int i = 0; i < poolSize; ++i) {
        int idx = (m_impl->nextSlot + i) % poolSize;
        if (!m_impl->sfxPool[idx].inUse || !ma_sound_is_playing(&m_impl->sfxPool[idx].sound)) {
            chosen = idx;
            break;
        }
    }

    SFXSlot& slot = m_impl->sfxPool[chosen];
    if (slot.inUse) {
        ma_sound_stop(&slot.sound);
        ma_sound_uninit(&slot.sound);
        slot.inUse = false;
    }

    ma_result result = ma_sound_init_from_file(
        &m_impl->engine, filePath,
        MA_SOUND_FLAG_ASYNC,    // non-blocking load — won't stall the game loop
        NULL, NULL, &slot.sound);

    if (result != MA_SUCCESS) {
        std::cerr << "AudioPlayer: failed to load SFX: " << filePath << "\n";
        return;
    }

    ma_sound_start(&slot.sound);
    slot.inUse = true;
    m_impl->nextSlot = (chosen + 1) % poolSize;
}

void AudioPlayer::setSFXVolume(float volume) {
    for (auto& slot : m_impl->sfxPool) {
        if (slot.inUse) ma_sound_set_volume(&slot.sound, volume);
    }
}

// ---------------------------------------------------------------- Music

void AudioPlayer::playMusic(const char* filePath, bool loop) {
    if (m_impl->musicLoaded) {
        ma_sound_stop(&m_impl->music);
        ma_sound_uninit(&m_impl->music);
        m_impl->musicLoaded = false;
    }

    ma_result result = ma_sound_init_from_file(
        &m_impl->engine, filePath,
        MA_SOUND_FLAG_STREAM,   // stream from disk — avoids loading whole file into RAM
        NULL, NULL, &m_impl->music);

    if (result != MA_SUCCESS) {
        std::cerr << "AudioPlayer: failed to load music: " << filePath << "\n";
        return;
    }

    ma_sound_set_looping(&m_impl->music, loop ? MA_TRUE : MA_FALSE);
    ma_sound_start(&m_impl->music);
    m_impl->musicLoaded = true;
}

void AudioPlayer::stopMusic() {
    if (m_impl->musicLoaded) {
        ma_sound_stop(&m_impl->music);
    }
}

void AudioPlayer::setMusicVolume(float volume) {
    if (m_impl->musicLoaded) ma_sound_set_volume(&m_impl->music, volume);
}