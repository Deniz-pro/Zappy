# Zappy Protocol (RFC)

## Table of Contents

1. [Introduction](#1-introduction)
2. [Connection](#2-connection)
3. [Communication](#3-communication)
4. [AI Client Commands](#4-ai-client-commands)
5. [GUI Protocol](#5-gui-protocol)
6. [Response Codes](#6-response-codes)

---

## 1. Introduction

The Zappy Protocol defines the TCP communication between:

- the server (zappy_server) and the GUI client (zappy_gui)
- the server (zappy_server) and the AI clients (zappy_ai)

The server is mono-process and mono-thread. It uses poll to handle socket multiplexing.

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

## 3. Communication

### 3.1 AI Client Handshake

When a client connects, the AI client and server exchange the following:

```
<-- WELCOME\n
--> TEAM-NAME\n
<-- CLIENT-NUM\n
<-- X Y\n
```

**X** and **Y** indicate the world's dimensions. **CLIENT-NUM** indicates the number of available slots on the server for the **TEAM-NAME** team. If this number is greater than or equal to 1, a new client can connect.

The client can send up to 10 requests in a row without any response from the server. Over 10, the server will no longer take them into account. The server executes the client's requests in the order they were received.

### 3.2 GUI Client Handshake

When a client connects, the GUI client and server exchange the following:

```
<-- WELCOME\n
--> GRAPHIC\n
```

The team name **GRAPHIC** is reserved for the GUI.

---

## 4. AI Client Commands

Each player (AI client) responds to the following commands only. All commands end with a newline character (\n).

An action's execution time is calculated as action_cost / f, where f is the frequency parameter (default: 100). For example with f=100, Forward takes 7/100 = 0.07 seconds.

| Command | Action | Time limit | Response |
|---------|--------|------------|----------|
| Forward | Move up one tile | 7/f | ok |
| Right | Turn 90 right | 7/f | ok |
| Left | Turn 90 left | 7/f | ok |
| Look | Look around | 7/f | [tile1, tile2, ...] |
| Inventory | Display inventory | 1/f | [food n, linemate n, ...] |
| Broadcast <text> | Broadcast a message | 7/f | ok |
| Connect_nbr | Number of unused team slots | - | value |
| Fork | Fork a player (lay an egg) | 42/f | ok |
| Eject | Eject players from tile | 7/f | ok/ko |
| Take <object> | Take object | 7/f | ok/ko |
| Set <object> | Set object | 7/f | ok/ko |
| Incantation | Start elevation ritual | 300/f | Elevation underway Current level: k/ko |

### 4.1 Forward
 
Moves the player one tile forward.
The world is toroidal: moving past the edge wraps around to the other side.
 
```
--> Forward
<-- ok
```
 
### 4.2 Right
 
Rotates the player 90° to the right without moving them.
 
```
--> Right
<-- ok
```
 
### 4.3 Left
 
Rotates the player 90° to the left without moving them.
 
```
--> Left
<-- ok
```


### 4.4 Look

The Look command returns the contents of all tiles visible by the player.
Vision range increases with level: at level 1 the player sees 1 tile ahead, at level 2 three tiles ahead, etc.

```
--> Look
<-- [player, object tile1, ..., object tileN]
```
Objects on the same tile are separated by a space. Tiles are separated by commas.

### 4.5 Inventory

Returns the content of the player's inventory along with remaining food units:

```
--> Inventory
<-- [food 345, linemate 2, deraumere 0, sibur 1, mendiane 0, phiras 0, thystame 0]
```

### 4.6 Broadcast

Broadcasts a text message to all players on the map.
All clients receive:

```
--> Broadcast text
<-- message K, text
```
where K is the tile number indicating the direction the sound is coming from (0 if same tile).

### 4.7 Connect_nbr
 
Returns the number of connection slots currently available for the player's team.
 
```
--> Connect_nbr
<-- value
```

### 4.8 Fork

Lays an egg, adding a new connection slot for the team. Once laid, the player continues normally.
A new AI client can then connect to the team using this slot.

```
--> Fork
<-- ok
```

### 4.9 Eject

Pushes all other players from the current tile of the client. It also destroys any eggs on the tile.

Ejected clients will receive:
```
<-- eject: K\n
```
where K is the direction from which the push came.

### 4.10 Take
 
Picks up the specified object from the current tile.

```
--> Take linemate
<-- ok
```
 
### 4.11 Set
 
Puts down the specified object from the player's inventory onto the current tile.
 
```
--> Set linemate
<-- ok
```

### 4.12 Incantation

The incantation starts an elevation ritual. All players of the same level on the tile are frozen for the duration (300/f).
Prerequisites (stones and player count) are checked at start and end of the ritual.

```
--> Incantation
<-- Elevation underway
<-- Current level: k
```

Elevation requirements:

| Elevation | Players | Linemate | Deraumere | Sibur | Mendiane | Phiras | Thystame |
|-----------|---------|----------|-----------|-------|----------|--------|----------|
| 1->2      | 1       | 1        | 0         | 0     | 0        | 0      | 0        |
| 2->3      | 2       | 1        | 1         | 1     | 0        | 0      | 0        |
| 3->4      | 2       | 2        | 0         | 1     | 0        | 2      | 0        |
| 4->5      | 4       | 1        | 1         | 2     | 0        | 1      | 0        |
| 5->6      | 4       | 1        | 2         | 1     | 3        | 0      | 0        |
| 6->7      | 6       | 1        | 2         | 3     | 0        | 1      | 0        |
| 7->8      | 6       | 2        | 2         | 2     | 2        | 2      | 1        |

---

## 5. GUI Protocol

The GUI client communicates with the server using a dedicated set of commands. After authentication with **GRAPHIC**, the server sends the full game state and then pushes events as they occur.

### Symbols

| Symbol | Meaning |
|--------|---------|
| X  | Width or horizontal position |
| Y  | Height or vertical position |
| q0 | Resource 0 (food) quantity |
| q1 | Resource 1 (linemate) quantity |
| q2 | Resource 2 (deraumere) quantity |
| q3 | Resource 3 (sibur) quantity |
| q4 | Resource 4 (mendiane) quantity |
| q5 | Resource 5 (phiras) quantity |
| q6 | Resource 6 (thystame) quantity |
| n  | Player number |
| O  | Orientation: 1(N), 2(E), 3(S), 4(W) |
| L  | Player or incantation level |
| e  | Egg number |
| T  | Time unit |
| N  | Name of the team |
| R  | Incantation result |
| M  | Message |
| i  | Resource number |

### Commands

| Server | Client | Description |
|--------|--------|-------------|
| msz X Y\n | msz\n | Map size |
| bct X Y q0 q1 q2 q3 q4 q5 q6\n | bct X Y\n | Content of a tile |
| bct X Y q0...q6\n * nbr_tiles | mct\n | Content of the map (all tiles) |
| tna N\n * nbr_teams | tna\n | Name of all the teams |
| pnw #n X Y O L N\n |  | Connection of a new player |
| ppo #n X Y O\n | ppo #n\n | Player's position |
| plv #n L\n | plv #n\n | Player's level |
| pin #n X Y q0...q6\n | pin #n\n | Player's inventory |
| pex #n\n |  | Expulsion of a player |
| pbc #n M\n |  | Broadcast message |
| pic X Y L #n #n ...\n |  | Start of an incantation |
| pie X Y R\n |  | End of an incantation |
| pfk #n\n |  | Egg laying by a player |
| pdr #n i\n |  | Resource dropping |
| pgt #n i\n |  | Resource collecting |
| pdi #n\n |  | Death of a player |
| enw #e #n X Y\n |  | An egg was laid by a player |
| ebo #e\n |  | Player connection for an egg |
| edi #e\n |  | Death of an egg |
| sgt T\n | sgt\n | Time unit request |
| sst T\n | sst T\n | Time unit modification |
| seg N\n | | End of game |
| smg M\n | | Message from the server |
| suc\n | | Unknown command |
| sbp\n | | Bad command parameter |

### 5.1 Map Size

Request the dimensions of the world.

```
--> msz\n
<-- msz X Y\n
```

### 5.2 Content of a Tile

Request the resources present on a specific tile at coordinates (X, Y).

```
--> bct X Y\n
<-- bct X Y q0 q1 q2 q3 q4 q5 q6\n
```

### 5.3 Content of All Tiles

Request the resource contents of every tile on the map.

```
--> mct\n
<-- bct 0 0 q0 q1 q2 q3 q4 q5 q6\n
    bct 0 1 q0 q1 q2 q3 q4 q5 q6\n
    ...
```

### 5.4 Name of All Teams

Request the list of all team names currently in the game.

```
--> tna\n
<-- tna Team1\n
    tna Team2\n
    ...
```

### 5.5 Connection of a New Player

Sent by the server automatically when a new AI client connects.

```
<-- pnw #n X Y O L N\n
```

### 5.6 Player Position

Request the current position and orientation of a player.

```
--> ppo #n\n
<-- ppo #n X Y O\n
```

### 5.7 Player Level

Request the current level of a player.

```
--> plv #n\n
<-- plv #n L\n
```

### 5.8 Player Inventory

Request the inventory of a player.

```
--> pin #n\n
<-- pin #n X Y q0 q1 q2 q3 q4 q5 q6\n
```

### 5.9 Player Expulsion

Sent by the server when a player executes `Eject`.

```
<-- pex #n\n
```

### 5.10 Player Broadcast

Sent by the server when a player broadcasts a message.

```
<-- pbc #n M\n
```

### 5.11 Start of an Incantation

Sent by the server when a player starts an incantation ritual.

```
<-- pic X Y L #n #n ...\n
```

### 5.12 End of an Incantation

Sent by the server when an incantation ends.

```
<-- pie X Y R\n
```

### 5.13 Egg Laying by a Player

Sent by the server when a player uses the `Fork` command.

```
<-- pfk #n\n
```

### 5.14 Egg Laid (with Position)

Sent by the server after doing the `pfk` command to provide the egg's ID and location.

```
<-- enw #e #n X Y\n
```

### 5.15 Player Connected via Egg

Sent when a new player spawns from a waiting egg.

```
<-- ebo #e\n
```

### 5.16 Death of an Egg

Sent when an egg is destroyed.

```
<-- edi #e\n
```

### 5.17 Resource Collecting

Sent when a player picks up a resource.

```
<-- pgt #n i\n
```

### 5.18 Resource Dropping

Sent when a player drops a resource onto a tile.

```
<-- pdr #n i\n
```

### 5.19 Death of a Player

Sent when a player dies of starvation (food = 0).

```
<-- pdi #n\n
```

### 5.20 Time Unit Request

Request the current frequency of the server.

```
--> sgt\n
<-- sgt T\n
```

### 5.21 Time Unit Modification

Change the frequency of the server.

```
--> sst T\n
<-- sst T\n
```

### 5.22 End of Game

Sent by the server when a team wins.

```
<-- seg N\n
```

### 5.23 Server Message

Sent by the server to notify a problem with a personalized message.

```
<-- smg M\n
```

### 5.24 Unknown Command

Sent by the server when the GUI sends an unrecognized command.

```
<-- suc\n
```

### 5.25 Bad Command Parameter

Sent by the server when a recognized GUI command is sent with invalid or missing parameters.

```
<-- sbp\n
```

---

## 6. Response Codes

The following responses are used by the server for AI client commands:

| Response | Meaning |
|----------|---------|
| ok | Request executed successfully |
| ko | Request failed (bad command, missing resource, wrong state, etc.) |
| dead | The player has died (starvation) |

For GUI commands, the server uses suc\n for unknown commands and sbp\n for bad parameters.

---

*End of RFC - Zappy Protocol (RFC)*
