# ZAPPY

The Zappy project is a networked game where teams of AI players compete to reach level 8 first. It consists of three independent binaries: a server that manages the game world, a GUI client for visualization, and an AI client that controls a player.

---

## Overview

Zappy is made of three independent binaries that communicate over a TCP protocol defined later:

| Binary | Language | Role |
|--------|----------|------|
| `zappy_server` | C++ | Generates and manages the game world |
| `zappy_gui` | C++ (lib Raylib) | Graphical visualization of the world |
| `zappy_ai` | Python (lib Pytorch) | AI client controlling a player |

---

## Usage

### Build

```bash
# Build all three binaries
make all
```

### Server

```bash
# Launch the server
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

**Example:**
```bash
./zappy_server -p 4242 -x 20 -y 20 -n team1 team2 -c 6 -f 100
```

### GUI Client

```bash
# Launch the GUI client
./zappy_gui --help
USAGE: ./zappy_gui -p port -h machine
```
| option | description |
|--------|-------------|
| -p port | port number |
| -h machine | hostname of the server |

**Example:**
```bash
./zappy_gui -p 4242 -h localhost
```

### AI Client

```bash
# Launch the AI client
./zappy_ai --help
USAGE: ./zappy_ai -p port -n name -h machine
```
| option | description |
|--------|-------------|
| -p port | port number |
| -n name | name of the team |
| -h machine | name of the machine; localhost by default |

**Example:**
```bash
./zappy_ai -p 4242 -n team1 -h localhost
```

---

## Game Rules

### World

The world consists of a flat and toroidal map (wraps around on all edges).

### Resources

Resources spawn at startup and every 20 time units, distributed evenly across the map.

| Resource | Density |
|----------|---------|
| Food | 0.5 |
| Linemate | 0.3 |
| Deraumere | 0.15 |
| Sibur | 0.1 |
| Mendiane | 0.1 |
| Phiras | 0.08 |
| Thystame | 0.05 |

> Quantity = `map_width × map_height × density`

### Survival

Each food unit keeps a player alive for **126 time units**. Players start with 10 food units (1260 time units).

### Elevation

Players level up by performing an **incantation**.

| Elevation | Players | Linemate | Deraumere | Sibur | Mendiane | Phiras | Thystame |
|-----------|---------|----------|-----------|-------|----------|--------|----------|
| 1->2      | 1       | 1        | 0         | 0     | 0        | 0      | 0        |
| 2->3      | 2       | 1        | 1         | 1     | 0        | 0      | 0        |
| 3->4      | 2       | 2        | 0         | 1     | 0        | 2      | 0        |
| 4->5      | 4       | 1        | 1         | 2     | 0        | 1      | 0        |
| 5->6      | 4       | 1        | 2         | 1     | 3        | 0      | 0        |
| 6->7      | 6       | 1        | 2         | 3     | 0        | 1      | 0        |
| 7->8      | 6       | 2        | 2         | 2     | 2        | 2      | 1        |

### Victory

The first team to have at least **6 players at level 8** wins.

---

## Protocol

The Zappy Protocol (RFC) is fully described in this [RFC documentation](https://github.com/EpitechPGE2-2025/G-YEP-400-PAR-4-1-zappy-2/blob/feature/documentation/docs/Zappy_Protocol.md).

---

## Contributors

- Abdoul-Latif WABI
- Deniz DEMIR
- Donata CONTANT
- Mahammad Abdourahman-Mohamed
- Marc-Antoine SADIO
