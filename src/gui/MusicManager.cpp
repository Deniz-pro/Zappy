/*
** EPITECH PROJECT, 2026
** ZAPPY
** File description:
** MusicManager.cpp
*/

#include "gui/MusicManager.hpp"

MusicManager::MusicManager()
{
    InitAudioDevice();
    _audioReady = IsAudioDeviceReady();
}

MusicManager::~MusicManager()
{
    shutdown();
}

void MusicManager::shutdown()
{
    stop();
    if (!_audioReady || !IsAudioDeviceReady())
        return;
    CloseAudioDevice();
    _audioReady = false;
}

void MusicManager::load(const std::string &path)
{
    if (_loaded)
        UnloadMusicStream(_music);
    _music = LoadMusicStream(path.c_str());
    _loaded = IsMusicReady(_music);
    if (!_loaded)
        TraceLog(LOG_WARNING, "Failed to load music: %s", path.c_str());
}

void MusicManager::play()
{
    if (!_loaded)
        return;
    PlayMusicStream(_music);
}

void MusicManager::stop()
{
    if (!_loaded)
        return;
    StopMusicStream(_music);
    UnloadMusicStream(_music);
    _loaded = false;
}

void MusicManager::update()
{
    if (_loaded)
        UpdateMusicStream(_music);
}

void MusicManager::setVolume(float volume)
{
    if (_loaded)
        SetMusicVolume(_music, volume);
}
