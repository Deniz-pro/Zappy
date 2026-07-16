# Zappy Dev Docs

## Table of Contents

1. [Introduction](#1-introduction)
2. [Connection](#2-connection)
3. [Add a new Command](#3-add-a-new-command)
4. [Add a new GUI Event](#4-add-a-new-gui-event)
5. [Documentation](#5-documentation)

---

## 1. Introduction

The Zappy Protocol defines the TCP communication between:

- the server (zappy_server) and the GUI client (zappy_gui)
- the server (zappy_server) and the AI clients (zappy_ai)

The server is mono-process and mono-thread. It uses poll to handle socket multiplexing.

**Dependencies**:

- Raylib
- Python
- C++

---

## 2. Connection

The client connects to the server via TCP on the specified port.

### 2.1 Server

```bash
./zappy_server --help
USAGE: ./zappy_server -p port -x width -y height -n name1 name2 ... -c clientsNb -f freq
```
| option | description |
|--------|-------------|
| -p port | port number |
| -x width | width of the world |
| -y height | height of the world |
| -n name1 name2 ... | name of the team |
| -c clientsNb | number of initial clients per team |
| -f freq | reciprocal of time unit for execution of actions |

### 2.2 GUI Client

```bash
./zappy_gui --help
USAGE: ./zappy_gui -p port -h machine
```
| option | description |
|--------|-------------|
| -p port | port number |
| -h machine | hostname of the server |

### 2.3 AI Client

```bash
./zappy_ai --help
USAGE: ./zappy_ai -p port -n name -h machine
```
| option | description |
|--------|-------------|
| -p port | port number |
| -n name | name of the team |
| -h machine | name of the machine; localhost by default |

---

## 3. Add a new Command

### 3.1 Adding a new AI Command

1. Add the new command in the enum `CommandType` :

```cpp
enum class CommandType {
    FORWARD, RIGHT, LEFT, LOOK,
    TAKE, SET, INVENTORY, FORK, EJECT,
    BROADCAST, CONNECT_NBR, INCANTATION,
    NEW_COMMAND // Add the new command right here
};
```

2. Add the new Command in the `CommandParser` :

```cpp
CommandParser::CommandParser() : _CommandMap({
        {"Forward", CommandType::FORWARD}, {"Right", CommandType::RIGHT},
        {"Left", CommandType::LEFT}, {"Look", CommandType::LOOK},
        {"Inventory", CommandType::INVENTORY},
        {"Broadcast", CommandType::BROADCAST},
        {"Connect_nbr", CommandType::CONNECT_NBR}, {"Fork", CommandType::FORK},
        {"Eject", CommandType::EJECT}, {"Take", CommandType::TAKE},
        {"Set", CommandType::SET}, {"Incantation", CommandType::INCANTATION},
        {"msz", CommandType::MSZ}, {"bct", CommandType::BCT},
        {"mct", CommandType::MCT}, {"tna", CommandType::TNA},
        {"ppo", CommandType::PPO}, {"plv", CommandType::PLV},
        {"pin", CommandType::PIN}, {"sgt", CommandType::SGT},
        {"sst", CommandType::SST}, {"newCommand", CommandType::NEW_COMMAND}, // Add the new command right here
    })
    {}
```

3. Implement the command in `AICommands.cpp` :

```cpp
void zappy::AICommands::init()
{
    /* ... */
    _cmds[CommandType::NEW_COMMAND] = [this](const std::string &)
    { sendNewCommand(); };
}

void zappy::AICommands::sendNewCommand(const std::string &cmd)
{
    Client *client = getClient();
    
    if (!client)
        return;
    /* logic code */
    client->toSend += "ok\n";
    client->response = true;
    _events.notifyPin(client->player); // notify the GUI on what is new
}
```

4. Declare the function in the `AICommands.hpp` :

```cpp
private:
    /* ... */
    void sendNewCommand(const std::string &cmd);
```

### 3.2 Add a new GUI Command

It's quite the same, but you must do these following changes in `GUICommands.cpp` :

```cpp
void zappy::GUICommands::init()
{
    /* ... */
    _cmds[CommandType::NEW_GUI_COMMAND] = [this](const std::string &cmd)
        { sendNewGUICommand(cmd); };
}

void zappy::GUICommands::sendNewGUICommand(const std::string &cmd)
{
    Client *client = getClient();

    if (!client)
        return;
    /* logic code */
    client->toSend += "the response\n";
}
```

> No matter if you are adding a new AI or GUI Command, you must implement it either in the GUI or in the AI.

---

## 4. Add a new GUI Event

1. Add the new event in `GUIEvents.hpp` :

```cpp
public:
    /* ... */
    void notifyNewEvent(const Player &p);
```

2. Implement the logic in `GUIEvents.cpp` :

```cpp
void zappy::GUIEvents::notifyNewEvent(const Player &p, int data)
{
    std::string event = "nev #" + std::to_string(p.id) + " "
        + std::to_string(data) + "\n";

    /* logic code */
    sendEvent(event);
}
```

3. Call the new function in an AI Command :

```cpp
void zappy::AICommands::sendFork() {

    Client *client = getClient();
    if (!client)
        return;
    /* ... */
    _gameState.addEgg(egg);
    _events.notifyNewEvent(client->player, egg.id); // notify the GUI with your new event
}
```

> If you are adding a new GUI Event, you must implement it either in the GUI.

---

## 5. Documentation

You can find more information on our other documentations:

- [Zappy RFC Protocol](https://github.com/EpitechPGE2-2025/G-YEP-400-PAR-4-1-zappy-2/blob/feature/documentation/docs/Zappy_Protocol.md).
- [Zappy Architecture](https://github.com/EpitechPGE2-2025/G-YEP-400-PAR-4-1-zappy-2/blob/feature/documentation/docs/Architecture.md).
