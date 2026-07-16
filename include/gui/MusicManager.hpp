/*
** EPITECH PROJECT, 2026
** ZAPPY
** File description:
** MusicManager.hpp
*/

#ifndef MUSICMANAGER_HPP_
    #define MUSICMANAGER_HPP_
    #include <raylib.h>
    #include <string>

class MusicManager {
    public:
        MusicManager();
        ~MusicManager();
        void load(const std::string &path);
        void play();
        void stop();
        void update();
        void setVolume(float volume);
        void shutdown();
    private:
        Music _music = {};
        bool _loaded = false;
        bool _audioReady = false;
};

#endif
