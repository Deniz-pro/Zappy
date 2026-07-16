/*
** EPITECH PROJECT, 2026
** ZAPPY
** File description:
** Main.cpp
*/

#include "Help.hpp"
#include "Epitech.hpp"
#include "Zappy.hpp"

int main(int ac, char **av)
{
    if (help(ac, av)) {
        print_help();
        return SUCCESS;
    }
    return Zappy::run(ac, av);
}
