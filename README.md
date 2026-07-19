# ZAPPY

The Zappy project is a networked game where teams of AI players compete to reach level 8 first. It consists of three independent binaries: a server that manages the game world, a GUI client for visualization, and an AI client that controls a player.

---

## Overview

Zappy is made of three independent binaries that communicate over a TCP protocol:

| Binary | Language | Role |
|--------|----------|------|
| `zappy_server` | C++ | Generates and manages the game world |
| `zappy_gui` | C++ (Raylib) | Graphical visualization of the world |
| `zappy_ai` | Python | AI client controlling a player |

---

## Build

```bash
make all
```

Individual targets:

```bash
make zappy_server
make zappy_gui
make zappy_ai
```

---

## Quick Start (full game)

Open **three separate terminals** and run each command in order:

**Terminal 1 — Server:**
```bash
./zappy_server -p 4242 -x 20 -y 20 -n team1 team2 -c 6 -f 100
```

**Terminal 2 — GUI:**
```bash
./zappy_gui -p 4242 -h localhost
```

**Terminal 3+ — AI clients (one per player, repeat for each):**
```bash
./start.sh -p 4242 -n team1 -h localhost
```

> The server must be started first. The GUI and AI can connect in any order after.
> Run `./install.sh` once before launching the AI for the first time.

---

## Server

```
./zappy_server -p port -x width -y height -n name1 name2 ... -c clientsNb -f freq
```

| Option | Description |
|--------|-------------|
| `-p port` | TCP port the server listens on |
| `-x width` | Width of the map (number of tiles) |
| `-y height` | Height of the map (number of tiles) |
| `-n name1 name2 ...` | Team names (at least one required) |
| `-c clientsNb` | Number of initial slots per team (how many AI can connect per team at game start) |
| `-f freq` | Game speed — number of actions executed per second. Higher = faster game |

**Example:**
```bash
./zappy_server -p 4242 -x 20 -y 20 -n team1 team2 -c 6 -f 100
```

> `-f 100` means 100 game ticks per second. Use lower values (e.g. `-f 2`) to slow the game down for debugging.

---

## GUI Client

```
./zappy_gui -p port -h machine
```

| Option | Description |
|--------|-------------|
| `-p port` | Port of the running server |
| `-h machine` | Hostname or IP of the server (`localhost` if running locally) |

**Example:**
```bash
./zappy_gui -p 4242 -h localhost
```

### Controls

| Action | Input |
|--------|-------|
| Rotate camera | Left click + drag |
| Zoom in/out | Scroll wheel |
| Click on player | Select and inspect a player |
| Click on team name | Expand/collapse team player list |
| Change game speed | `+` / `-` buttons in the top-right panel |

---

## AI Client

The AI client requires a Python virtual environment. Run the installer once before the first launch:

```bash
./install.sh
```

This creates a `.venv/` directory and installs the required dependencies (`textual`, `rich`, `requests`).

Then launch the AI with:

```
./start.sh -p port -n name [-h machine]
```

| Option | Description |
|--------|-------------|
| `-p port` | Port of the running server |
| `-n name` | Team name the AI joins (must match a name given to the server with `-n`) |
| `-h machine` | Hostname or IP of the server (`localhost` by default) |

**Example:**
```bash
./start.sh -p 4242 -n team1
./start.sh -p 4242 -n team1 -h 192.xxx.x.xx
```

> Launch multiple instances to fill a team. Each instance controls one player independently.
> You only need to run `./install.sh` once per machine.

---

## Game Rules

### World

The world is a flat toroidal map (wraps around on all edges). Tiles contain resources that players collect to perform incantations.

### Resources

Resources spawn at startup and are periodically replenished across the map.

| Resource | Density |
|----------|---------|
| Food | 0.5 |
| Linemate | 0.3 |
| Deraumere | 0.15 |
| Sibur | 0.1 |
| Mendiane | 0.1 |
| Phiras | 0.08 |
| Thystame | 0.05 |

> Quantity on map = `width × height × density`

### Survival

Each food unit keeps a player alive for **126 time units**. Players start with 10 food units (1260 time units of life). The AI must continuously gather food to survive.

### Elevation (leveling up)

Players level up by performing an **incantation** — standing on the same tile as the required resources and the required number of players at the same level.

| Level | Players needed | Linemate | Deraumere | Sibur | Mendiane | Phiras | Thystame |
|-------|---------------|----------|-----------|-------|----------|--------|----------|
| 1→2   | 1             | 1        | 0         | 0     | 0        | 0      | 0        |
| 2→3   | 2             | 1        | 1         | 1     | 0        | 0      | 0        |
| 3→4   | 2             | 2        | 0         | 1     | 0        | 2      | 0        |
| 4→5   | 4             | 1        | 1         | 2     | 0        | 1      | 0        |
| 5→6   | 4             | 1        | 2         | 1     | 3        | 0      | 0        |
| 6→7   | 6             | 1        | 2         | 3     | 0        | 1      | 0        |
| 7→8   | 6             | 2        | 2         | 2     | 2        | 2      | 1        |

### Victory

The first team to have at least **6 players at level 8** wins.

---

## Protocol

The full Zappy TCP protocol is documented in the [RFC documentation](https://github.com/EpitechPGE2-2025/G-YEP-400-PAR-4-1-zappy-2/blob/feature/documentation/docs/Zappy_Protocol.md).

---

## Contributors

- Abdoul-Latif WABI
- Deniz DEMIR
- Donata CONTANT
- Mahammad Abdourahman-Mohamed
- Marc-Antoine SADIO
