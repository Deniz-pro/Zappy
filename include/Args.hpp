/*
** EPITECH PROJECT, 2026
** ZAPPY
** File description:
** Args.hpp
*/

#ifndef ARGS_HPP
    #define ARGS_HPP
    #include <string>

class Args {
    public:
        static Args parse(int ac, char **av);
        int getPort() const
        {
            return _port;
        }
        const std::string &getHost() const
        {
            return _host;
        }
    private:
        int _port = -1;
        std::string _host = "localhost";
        static int parsePort(const char *value);
        static Args fill(int ac, char **av);
};

#endif
