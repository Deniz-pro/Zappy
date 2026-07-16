/*
** EPITECH PROJECT, 2026
** ZAPPY
** File description:
** Run.cpp
*/

#include "Epitech.hpp"
#include "Zappy.hpp"
#include "Args.hpp"
#include "Window.hpp"
#include "gui/network/Client.hpp"
#include <iostream>

void Zappy::launch(int ac, char **av)
{
    Args args = Args::parse(ac, av);
    Client client;

    client.connectTo(args.getHost(), args.getPort());
    client.handshake();
    Window window({}, client);
    window.run();
}

int Zappy::run(int ac, char **av)
{
    try {
        launch(ac, av);
    } catch (const std::exception &e) {
        std::cerr << "Error: " << e.what() << "\n";
        return ERROR_EXIT;
    }
    return SUCCESS;
}
