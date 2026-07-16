/*
** EPITECH PROJECT, 2026
** Zappy
** File description:
** test_commands.cpp
*/

#include <criterion/criterion.h>
#include "state/Player.hpp"
#include "state/GameState.hpp"
#include "state/Tile.hpp"

Test(orientation, right_north_to_east)
{
    int ori = static_cast<int>(Orientation::NORTH);
    Orientation result = static_cast<Orientation>(ori % 4 + 1);
    
    cr_assert_eq(result, Orientation::EAST);
}

Test(orientation, right_east_to_south)
{
    int ori = static_cast<int>(Orientation::EAST);
    Orientation result = static_cast<Orientation>(ori % 4 + 1);
    
    cr_assert_eq(result, Orientation::SOUTH);
}

Test(orientation, right_south_to_west)
{
    int ori = static_cast<int>(Orientation::SOUTH);
    Orientation result = static_cast<Orientation>(ori % 4 + 1);
    
    cr_assert_eq(result, Orientation::WEST);
}

Test(orientation, right_west_to_north)
{
    int ori = static_cast<int>(Orientation::WEST);
    Orientation result = static_cast<Orientation>(ori % 4 + 1);
    
    cr_assert_eq(result, Orientation::NORTH);
}

Test(orientation, left_north_to_west)
{
    int ori = static_cast<int>(Orientation::NORTH);
    Orientation result = static_cast<Orientation>((ori +2) % 4 + 1);
    
    cr_assert_eq(result, Orientation::WEST);
}

Test(orientation, left_west_to_south)
{
    int ori = static_cast<int>(Orientation::WEST);
    Orientation result = static_cast<Orientation>((ori +2) % 4 + 1);
    
    cr_assert_eq(result, Orientation::SOUTH);
}

Test(orientation, right_full_cycle)
{
    Orientation ori = Orientation::NORTH;

    for (int i = 0; i < 4; i++) {
        int o = static_cast<int>(ori);
        ori = static_cast<Orientation>(o % 4 + 1);
    }
    cr_assert_eq(ori, Orientation::NORTH);
}

Test(orientation, left_full_cycle)
{
    Orientation ori = Orientation::NORTH;

    for (int i = 0; i < 4; i++) {
        int o = static_cast<int>(ori);
        ori = static_cast<Orientation>((o + 2) % 4 + 1);
    }
    cr_assert_eq(ori, Orientation::NORTH);
}

Test(player, initial_food)
{
    Player p;

    p.inventory[0] = 10;
    cr_assert_eq(p.inventory[0], 10);
}

Test(player, hunger_decrements)
{
    Player p;

    p.inventory[0] = 10;
    p.inventory[0]--;
    cr_assert_eq(p.inventory[0], 9);
}

Test(player, hunger_death)
{
    Player p;

    p.inventory[0] = 1;
    p.inventory[0]--;
    cr_assert_leq(p.inventory[0], 0);
}

Test(player, initial_level)
{
    Player p;

    cr_assert_eq(p.level, 1);
}

Test(player, initial_position)
{
    Player p;

    cr_assert_eq(p.x, 0);
    cr_assert_eq(p.y, 0);
}

Test(player, initial_orientation)
{
    Player p;

    cr_assert_eq(p.orientation, Orientation::NORTH);
}

Test(player, inventory_take_resource)
{
    Player p;

    p.inventory[1] = 0;
    p.inventory[1]++;
    cr_assert_eq(p.inventory[1], 1);
}

Test(player, inventory_set_resource)
{
    Player p;

    p.inventory[1] = 1;
    p.inventory[1]--;
    cr_assert_eq(p.inventory[1], 0);
}

Test(gameState, map_size)
{
    GameState gs;

    gs.setMapSize(10, 10);
    cr_assert_eq(gs.getMapWidth(), 10);
    cr_assert_eq(gs.getMapHeight(), 10);
}

Test(gameState, add_player)
{
    GameState gs;
    Player pl;
    int id;

    pl.team = "team1";
    id = gs.addPlayer(pl);
    cr_assert_eq(gs.getPlayers().size(), 1);
    cr_assert_eq(gs.getPlayers().at(id).team, "team1");
}

Test(gameState, remove_player)
{
    GameState gs;
    Player pl;
    int id;

    id = gs.addPlayer(pl);
    gs.removePlayer(id);
    cr_assert_eq(gs.getPlayers().size(), 0);
}

Test(gameState, set_tile_resource)
{
    GameState gs;
    int res[7] = {3, 0, 0, 0, 0, 0, 0};

    gs.setMapSize(5, 5);
    gs.setTileResources(2, 2, res);
    cr_assert_eq(gs.getTiles()[2][2].resources[0], 3);
}

Test(gameState, team_slots)
{
    GameState gs;

    gs.addTeam("team1");
    gs.teamSlots["team1"] = 5;
    cr_assert_eq(gs.teamSlots["team1"], 5);
}

Test(gameState, multiple_players)
{
    GameState gs;
    Player p1;
    Player p2;

    p1.team = "team1";
    p2.team = "team2";
    gs.addPlayer(p1);
    gs.addPlayer(p2);
    cr_assert_eq(gs.getPlayers().size(), 2);
}

Test(forward, wrap_north)
{
    Player p;
    int h;

    p.x = 0;
    p.y = 0;
    p.orientation = Orientation::NORTH;
    h = 10;
    p.y = (p.y - 1 + h) % h;
    cr_assert_eq(p.y, 9);
}

Test(forward, wrap_east)
{
    Player p;
    int w;

    p.x = 9;
    p.y = 0;
    p.orientation = Orientation::EAST;
    w = 10;
    p.x = (p.x + 1) % w;
    cr_assert_eq(p.x, 0);
}

Test(forward, wrap_south)
{
    Player p;
    int h;

    p.x = 0;
    p.y = 9;
    p.orientation = Orientation::SOUTH;
    h = 10;
    p.y = (p.y + 1) % h;
    cr_assert_eq(p.y, 0);
}

Test(forward, wrap_west)
{
    Player p;
    int w;

    p.x = 0;
    p.y = 0;
    p.orientation = Orientation::WEST;
    w = 10;
    p.x = (p.x - 1 + w) % w;
    cr_assert_eq(p.x, 9);
}