##
## EPITECH PROJECT, 2025
## G-YEP-400-PAR-4-1-zappy-2
## File description:
## PRO_IA.py
##

import random
from src.Zappy_ai.Utils.choice_ressources import pick_priority_target_v2
from src.Zappy_ai.Utils.find_ressources import scan_resources, coords_cost_need
from src.Zappy_ai.Utils.elevation import missing_stones, get_req

Command_need_handle = ["Inventory", "Look", "Connect_nbr"]
FOOD_CRITICAL       = 10
FOOD_MIN_ELEV       = 20
FOOD_MIN_COORD      = 8
WAIT_TURNS          = 30

STATE_INIT          = "init"
STATE_COLLECT       = "collect"
STATE_SEEK_PARTNER  = "seek_partner"
STATE_GOTO_RENDEZ   = "goto_rendez"
STATE_WAIT_PARTNER  = "wait_partner"
STATE_BROADCAST     = "broadcast"
STATE_DROP_STONES   = "drop_stones"
STATE_INCANTATION   = "incantation"

EXPLORE_MOVES       = ["Forward", "Forward", "Forward", "Right", "Left"]


class PRO_IA:

    def __init__(self, team: str):
        self.team               = team
        self.the_map            = {}
        self.inventory          = {}
        self.visible            = {}
        self.objectif           = None
        self.objectif_coords    : tuple = None
        self.param_is_change    = False
        self.is_in_elev         = False
        self.after_underway     = False
        self.nbr_move           = 0
        self.nb_steps           = 0
        self.Connect_nbr        = 0
        self.level              = 1
        self.next_command       = None
        self._pending_takes     : list[str] = []
        self._waiting_look      = False
        self.nbr_of_incantation = 0
        self.skip_incantation   = 0
        self._last_snapshot     = None
        self.total_forks = 0
        self.want_switch = False
        
        # --- coordination ---
        self.state              = STATE_INIT
        self.rendez_row         = 0
        self.wait_counter       = 0
        self._pending_sets      : list[str] = []
        self.confirmed_levels   : list[int] = []
        self.i_launched_incantation = False
        self.partner_tile       = None

    # ------------------------------------------------------------------
    # Handlers
    # ------------------------------------------------------------------

    def handle_command(self, command: str, result):
        if command in Command_need_handle:
            self.param_is_change = True
        if command == "Inventory":
            self.inventory = result
            # if self.inventory == {} :
            #     self.next_command = "Inventory"
        elif command == "Look":
            # if self._players_on_current_tile() < 1 :
            #     self.next_command = "Look"
            self._waiting_look = False
            self.the_map = result
            # si un joueur est déjà sur ma case et que j'ai tout -> broadcast direct
            if (self.state not in (STATE_BROADCAST, STATE_DROP_STONES, STATE_INCANTATION)
                    and self._players_on_current_tile() > 1
                    and self.level > 1
                    and self._has_all_stones()
                    and self.inventory.get("food", 0) > FOOD_CRITICAL):
                self.state            = STATE_BROADCAST
                self.wait_counter     = 0
                self.confirmed_levels = []
                self.next_command     = f"Broadcast lvl:{self.level}"
            else :
                if (self.state not in (STATE_BROADCAST, STATE_DROP_STONES, STATE_INCANTATION)
                        and self._players_on_current_tile() < 2) :
                    self._queue_current_tile_takes()
        elif command == "Connect_nbr":
            self.Connect_nbr = result
        elif command == "Incantation":
            if result == "underway":
                self.after_underway = True
            elif result is None:
                self.is_in_elev = False
                self.nbr_of_incantation += 1
                self._on_incantation_failed()
            elif isinstance(result, int):
                self.level = result
                self.is_in_elev = False
                self._on_incantation_success()
        elif command.startswith("Set"):
            pass

    def handle_broadcast(self, direction: int, text: str):
        if direction != 0:
            return
        text = text.strip()
        if text.startswith("lvl:"):
            try:
                lvl = int(text.split(":")[1].strip())
                self.confirmed_levels.append(lvl)
            except ValueError:
                pass


    def handle_elevation_underway(self):
        print("[EVENT] Elevation underway reçu -> gèle en attente du résultat")
        self.is_in_elev     = True
        self.after_underway = True
        self.state          = STATE_INCANTATION

    def handle_elevation_result(self, new_level: int | None):
        if new_level is not None:
            print(f"[EVENT] Elevation réussie (passif) -> nouveau level {new_level}")
            self.level = new_level
            self._on_incantation_success()
        else:
            print("[EVENT] Elevation échouée (passif)")
            self.is_in_elev     = False
            self.after_underway = False
            self._go_collect()

    # ------------------------------------------------------------------
    # Helpers internes
    # ------------------------------------------------------------------

    def _queue_current_tile_takes(self):
        if not self.the_map:
            return
        current_tile: list = self.the_map.get(0, [])
        takes = []
        for item in current_tile:
            if item == "player" or item.startswith("player-"):
                continue
            takes.append("Take " + item)
        if len(takes) > 2 :
            takes.append("Inventory")
        self._pending_takes = takes + self._pending_takes

    def _has_all_stones(self) -> bool:
        return sum(missing_stones(self.level, self.inventory).values()) == 0

    def _players_on_current_tile(self) -> int:
        tile = self.the_map.get(0, [])
        return sum(1 for item in tile if item == "player" or item.startswith("player-"))

    def _find_player_tile(self) -> tuple | None:
        best_index   = None
        best_players = 0
        best_cost    = float('inf')
        for tile_index, contents in self.the_map.items():
            if tile_index == 0:
                continue
            count = sum(1 for item in contents if item == "player" or item.startswith("player-"))
            if count == 0:
                continue
            from src.Zappy_ai.Utils.find_ressources import moves_to_tile
            cost = moves_to_tile(tile_index)
            if count > best_players or (count == best_players and cost < best_cost):
                best_players = count
                best_index   = tile_index
                best_cost    = cost
        if best_index is None:
            return None
        return (best_index, best_players)

    def _build_drop_list(self) -> list[str]:
        req = get_req(self.level)
        if req is None:
            return []
        sets = []
        for stone in ["linemate", "deraumere", "sibur", "mendiane", "phiras", "thystame"]:
            needed = getattr(req, stone)
            for _ in range(needed):
                sets.append("Set " + stone)
        return sets

    def _enough_same_level(self) -> bool:
        req = get_req(self.level)
        if req is None:
            return False
        same = sum(1 for lvl in self.confirmed_levels if lvl == self.level)
        return (same + 1) >= req.players

    def _go_collect(self):
        self.state            = STATE_COLLECT
        self.rendez_row       = 0
        self.wait_counter     = 0
        self._pending_sets    = []
        self.confirmed_levels = []
        self.partner_tile     = None
        self.reset_all()

    def _on_incantation_success(self):
        self.state            = STATE_COLLECT
        self.confirmed_levels = []
        self._pending_sets    = []
        self.reset_all()

    def _on_incantation_failed(self):
        self.next_command     = 'Broadcast failed'
        self.state            = STATE_COLLECT
        self.confirmed_levels = []
        self._pending_sets    = []
        self.reset_all()

    def _explore(self) -> str:
        """Mouvement d'exploration aléatoire, biaisé vers Forward."""
        return random.choice(EXPLORE_MOVES)

    # ------------------------------------------------------------------
    # Public
    # ------------------------------------------------------------------

    def add_level(self):
        self.level += 1

    def get_state(self):
        return self.is_in_elev

    def set_state(self, state: bool):
        self.is_in_elev = state

    def reset_all(self):
        self.the_map         = {}
        self.inventory       = {}
        self.visible         = {}
        self.objectif        = None
        self.nb_steps        = 0
        self.objectif_coords = None
        self.is_in_elev      = False
        self.nbr_move        = 0
        self.param_is_change = False
        self._pending_takes  = []
        self._waiting_look   = False

    # ------------------------------------------------------------------
    # Machine à états principale
    # ------------------------------------------------------------------

    def take_decision(self) -> str | None:

        snapshot = {
            "state":            self.state,
            "level":            self.level,
            "food":             self.inventory.get("food", "?"),
            "objectif":         self.objectif[0] if self.objectif else None,
            "is_in_elev":       self.is_in_elev,
            "pending_takes":    len(self._pending_takes),
            "pending_sets":     len(self._pending_sets),
            "confirmed_levels": list(self.confirmed_levels),
            "wait_counter":     self.wait_counter,
            "missing_stones":   missing_stones(self.level, self.inventory) if self.inventory else {},
        }
        if snapshot != self._last_snapshot:
            self._last_snapshot = snapshot
            print(
                f"\n===== ÉTAT DU JOUEUR =====\n"
                f"  State           : {snapshot['state']}\n"
                f"  Level           : {snapshot['level']}\n"
                f"  Food            : {snapshot['food']}\n"
                f"  Objectif        : {snapshot['objectif']}\n"
                f"  Pierres manq.   : {snapshot['missing_stones']}\n"
                f"  En incantation  : {snapshot['is_in_elev']}\n"
                f"  Pending takes   : {snapshot['pending_takes']}\n"
                f"  Pending sets    : {snapshot['pending_sets']}\n"
                f"  Levels confirmés: {snapshot['confirmed_levels']}\n"
                f"  Wait counter    : {snapshot['wait_counter']}\n"
                f"========================="
            )

        if self.is_in_elev:
            return None

        if self.skip_incantation > 5:
            self.nbr_of_incantation = 0
            self.skip_incantation   = 0

        if self.next_command is not None:
            command           = self.next_command
            self.next_command = None
            return command

        if self.nb_steps == 0:
            self.nb_steps += 1
            self.inventory = {}
            return "Inventory"
        if self.nb_steps == 1:
            self.nb_steps += 1
            self.the_map = {}
            return "Look"

        if self.inventory == {} or self.the_map == {}:
            return None

        if self._pending_takes:
            return self._pending_takes.pop(0)

        if self._pending_sets:
            return self._pending_sets.pop(0)

        food = self.inventory.get("food", 0)
        if food < FOOD_CRITICAL and self.state != STATE_COLLECT:
            self._go_collect()

        # ── STATE_INIT ────────────────────────────────────────────────
        if self.state == STATE_INIT:
            self.state = STATE_COLLECT

        # ── STATE_COLLECT ─────────────────────────────────────────────
        if self.state == STATE_COLLECT:

            if self.level == 1:
                if self._has_all_stones() and food >= 4:
                    if self.nbr_of_incantation < 5:
                        self.state         = STATE_DROP_STONES
                        self._pending_sets = self._build_drop_list()
                        if self._pending_sets:
                            return self._pending_sets.pop(0)
                        self.is_in_elev = True
                        self.state      = STATE_INCANTATION
                        self.i_launched_incantation = True
                        return "Incantation"
                    else:
                        self.skip_incantation += 1

            if self._has_all_stones() and food >= FOOD_MIN_ELEV:
                self.state = STATE_SEEK_PARTNER
                if not self._waiting_look:
                    self._waiting_look = True
                    self.next_command  = "Look"
                    self.the_map       = {}
                return self._explore()

            return self._collect_step()

        # ── STATE_SEEK_PARTNER ────────────────────────────────────────
        if self.state == STATE_SEEK_PARTNER:

            if not self._has_all_stones() or food < FOOD_MIN_ELEV:
                self._go_collect()
                return self._collect_step()

            player_info = self._find_player_tile()

            if player_info is not None:
                tile_index, nb_players = player_info
                req            = get_req(self.level)
                players_needed = req.players if req else 1

                # groupe déjà complet -> ignorer
                if nb_players >= players_needed:
                    if not self._waiting_look:
                        self._waiting_look = True
                        self.next_command  = "Look"
                        self.the_map       = {}
                    return self._explore()

                self.partner_tile = tile_index

                if nb_players >= 2:
                    self.state           = STATE_GOTO_RENDEZ
                    self.objectif_coords = coords_cost_need(tile_index)
                    self.nbr_move        = 0
                    return self._move_toward(self.objectif_coords)

                from src.Zappy_ai.Utils.find_ressources import tile_index_to_coords
                _, row          = tile_index_to_coords(tile_index)
                self.rendez_row = row
                self.state      = STATE_GOTO_RENDEZ
                self.nbr_move   = 0
                return self._move_forward_n(row)

            if not self._waiting_look:
                self._waiting_look = True
                self.next_command  = "Look"
                self.the_map       = {}
            return self._explore()

        # ── STATE_GOTO_RENDEZ ─────────────────────────────────────────
        if self.state == STATE_GOTO_RENDEZ:

            if food < FOOD_MIN_COORD:
                self._go_collect()
                return self._collect_step()

            if self.nbr_move < self.rendez_row:
                self.nbr_move += 1
                return "Forward"

            self.state        = STATE_WAIT_PARTNER
            self.wait_counter = 0
            if not self._waiting_look:
                self._waiting_look = True
                self.the_map       = {}
            return "Look"

        # ── STATE_WAIT_PARTNER ────────────────────────────────────────
        if self.state == STATE_WAIT_PARTNER:

            if food < FOOD_MIN_COORD:
                self._go_collect()
                return self._collect_step()

            players_here = self._players_on_current_tile()

            if players_here >= 1:
                self.state            = STATE_BROADCAST
                self.wait_counter     = 0
                self.confirmed_levels = []
                return f"Broadcast lvl:{self.level}"

            self.wait_counter += 1
            if self.wait_counter >= WAIT_TURNS:
                self.state = STATE_SEEK_PARTNER
                if not self._waiting_look:
                    self._waiting_look = True
                    self.next_command  = "Look"
                    self.the_map       = {}
                return self._explore()

            if not self._waiting_look:
                self._waiting_look = True
                self.the_map       = {}
            return "Look"

        # ── STATE_BROADCAST ───────────────────────────────────────────
        if self.state == STATE_BROADCAST:

            if food < FOOD_MIN_COORD:
                self.next_command = 'Broadcast failed'
                self._go_collect()
                return self._collect_step()

            players_here = self._players_on_current_tile()

            if players_here == 1:
                self.state = STATE_SEEK_PARTNER
                if not self._waiting_look:
                    self._waiting_look = True
                    self.next_command  = "Look"
                    self.the_map       = {}
                return self._explore()

            req = get_req(self.level)
            if req is None:
                self._go_collect()
                return self._collect_step()

            self.wait_counter += 1
            if self.wait_counter % 10 == 0:
                if not self._waiting_look:
                    self._waiting_look = True
                    self.next_command  = "Look"
                    self.the_map       = {}
                    self.inventory = {}
                return "Inventory"

            if self._enough_same_level():
                if (players_here + 1) >= req.players:
                    self.state         = STATE_DROP_STONES
                    self._pending_sets = self._build_drop_list()
                    if self._pending_sets:
                        return self._pending_sets.pop(0)
                    self.state      = STATE_INCANTATION
                    self.is_in_elev = True
                    self.i_launched_incantation = True
                    return "Incantation"

            return f"Broadcast lvl:{self.level}"

        # ── STATE_DROP_STONES ─────────────────────────────────────────
        if self.state == STATE_DROP_STONES:
            self.state      = STATE_INCANTATION
            self.is_in_elev = True
            self.i_launched_incantation = True
            return "Incantation"

        # ── STATE_INCANTATION ─────────────────────────────────────────
        if self.state == STATE_INCANTATION:
            self.is_in_elev = True
            return None

        return self._explore()

    # ------------------------------------------------------------------
    # Helpers déplacement
    # ------------------------------------------------------------------

    def _move_toward(self, coords: tuple) -> str:
        col_abs, row, direction = coords
        if self.nbr_move < row:
            self.nbr_move += 1
            return "Forward"
        if self.nbr_move == row and col_abs > 0:
            self.nbr_move += 1
            return direction
        self.nbr_move += 1
        return "Forward"

    def _move_forward_n(self, n: int) -> str:
        if self.nbr_move < n:
            self.nbr_move += 1
            return "Forward"
        self.state        = STATE_WAIT_PARTNER
        self.wait_counter = 0
        return "Forward"

    # ------------------------------------------------------------------
    # Collecte classique
    # ------------------------------------------------------------------

    def _collect_step(self) -> str:

        food = self.inventory.get("food", 0)

        if food < FOOD_CRITICAL and self.objectif is not None:
            if self.objectif[0] != "food":
                self.visible = scan_resources(self.the_map)
                food_info    = self.visible.get("food")
                if food_info is not None:
                    self.objectif        = ("food", food_info[0], food_info[1])
                    self.objectif_coords = (food_info[0], food_info[1], "Take food")
                else:
                    self.reset_all()
                    return self._explore()

        if self.param_is_change:
            self.param_is_change = False
            self.nbr_move        = 0
            self.visible         = scan_resources(look_result=self.the_map)
            self.objectif        = pick_priority_target_v2(
                inventory   = self.inventory,
                visible     = self.visible,
                level       = self.level,
                look_result = self.the_map
            )
            if self.objectif is not None:
                self.objectif_coords = coords_cost_need(self.objectif[1])

        # pas d'objectif -> exploration aléatoire
        if self.objectif is None:
            if not self._waiting_look:
                self._waiting_look = True
                self.next_command  = "Look"
                self.the_map       = {}
            return self._explore()

        if self.nbr_move < self.objectif_coords[1]:
            self.nbr_move += 1
            return "Forward"

        if self.nbr_move == self.objectif_coords[1]:
            self.nbr_move += 1
            return self.objectif_coords[2]

        if self.nbr_move > self.objectif_coords[1]:
            ressource = self.objectif[0]
            self.reset_all()
            return "Take " + ressource

        return self._explore()