/*
** EPITECH PROJECT, 2026
** G-YEP-400-PAR-4-1-zappy-2
** File description:
** ParseArgs.hpp
*/

#ifndef PARSEARGS_HPP
    #define PARSEARGS_HPP
    #include <functional>
    #include <unistd.h>
    #include <unordered_map>
    #include <vector>
    #include <string>

namespace zappy {
    struct Params {
        int port = 4242;
        int width = 10;
        int height = 10;
        std::vector<std::string> teams;
        int clientsNb = 1;
        int freq = 100;
    };

    class ParseArgs {
        using Opt = std::function<void()>;
        std::unordered_map<char, Opt> _flags;
        void getTeamsNames(Params &params);
        void manageFlags(Params &params);
        void managePort(Params &params);
        char **_argv;
        int _argc;
    public:
        ParseArgs(int argc, char **argv);
        Params parseFlags();
    };
}

#endif
