/*
** EPITECH PROJECT, 2026
** ZAPPY
** File description:
** GameState.hpp
*/

#ifndef GAMESTATE_HPP_
    #define GAMESTATE_HPP_
    #include "state/Egg.hpp"
    #include "state/Tile.hpp"
    #include "state/Player.hpp"
    #include "state/Incantation.hpp"
    #include "state/Broadcast.hpp"
    #include <string>
    #include <vector>
    #include <queue>
    #include <deque>
    #include <unordered_map>

class GameState {
    public:
        int freq = 0;
        int selectedPlayerId = -1;
        std::string selectedTeam = "";
        std::unordered_map<std::string, int> teamSlots;
        int nextId = 1;
        int nextEgg = 1;
        int getMapWidth() const
        {
            return _mapWidth;
        }
        int getMapHeight() const
        {
            return _mapHeight;
        }
        bool isMapChanged() const
        {
            return _mapChanged;
        }
        const std::vector<std::string> &getTeams() const
        {
            return _teams;
        }
        const std::unordered_map<int, Player> &getPlayers() const
        {
            return _players;
        }
        const std::vector<std::vector<Tile>> &getTiles() const
        {
            return _tiles;
        }
        int addPlayer(Player p)
        {
            p.id = nextId++;
            _players[p.id] = p;
            return p.id;
        }
        int addEgg(const zappy::Egg &egg)
        {
            zappy::Egg e = egg;
            e.id = nextEgg++;
            _eggs[e.id] = e;
            return e.id;
        }
        void removeEgg(int id) { _eggs.erase(id); }
        void setEgg(const zappy::Egg &egg) { _eggs[egg.id] = egg; }
        void registerEgg(int id, int playerId, int x, int y);
        const std::unordered_map<int, zappy::Egg> &getEggs() const { return _eggs; }
        void setMapSize(int width, int height);
        void resetMapChanged();
        void addTeam(const std::string &team);
        void setPlayer(const Player &player);
        void removePlayer(int id);
        void setTileResources(int x, int y, const int resources[7]);
        void decrementTileResource(int x, int y, int index);
        void incrementTileResource(int x, int y, int index);
        void incrementPlayerResource(int id, int index);
        void decrementPlayerResource(int id, int index);
        void setEggAnim(int id, zappy::EggAnim anim);
        void addIncantation(int x, int y, int level,
            const std::vector<int> &playerIds);
        void endIncantation(int x, int y, bool success);
        void addExpulsion(int x, int y);
        void pushBroadcast(int playerId, const std::string &text);
        void setWinner(const std::string &team);
        void setServerMessage(const std::string &msg, float duration);
        void pushBroadcast(int playerId, const BroadcastMessage &msg)
        {
            _broadcasts[playerId].push_back(msg);
        }
        void pushIncantationMessage(int playerId,
            const IncantationMessage &msg);
        bool popIncantationMessage(int playerId,
            IncantationMessage &msg);
        bool popBroadcast(int playerId, BroadcastMessage &msg);
        const std::string &getWinner() const
        {
            return _winner;
        }
        const std::string &getServerMessage() const
        {
            return _serverMessage;
        }
        void tickEffects(float dt);
        const std::unordered_map<int, IncantationState> &
        getIncantations() const
        {
            return _incantations;
        }
        const std::unordered_map<int, float> &
        getExpulsions() const
        {
            return _expulsions;
        }
        const std::unordered_map<int, std::deque<BroadcastMessage>> &
        getBroadcasts() const
        {
            return _broadcasts;
        }
    private:
        int _mapWidth = 0;
        int _mapHeight = 0;
        bool _mapChanged = false;
        std::vector<std::string> _teams;
        std::unordered_map<int, Player> _players;
        std::vector<std::vector<Tile>> _tiles;
        std::unordered_map<int, zappy::Egg> _eggs;
        std::unordered_map<int, IncantationState> _incantations;
        std::unordered_map<int, float> _expulsions;
        std::unordered_map<int, std::deque<BroadcastMessage>> _broadcasts;
        std::unordered_map<int, std::queue<IncantationMessage>> _incantations_msg;
        std::string _winner;
        std::string _serverMessage;
        float _serverMessageTimer = 0.0f;
        void resizeMap();
        void tickIncantations(float dt);
        void tickExpulsions(float dt);
        void tickEggs(float dt);
        void tickBroadcasts(float dt);
        int tileKey(int x, int y) const;
};

#endif
