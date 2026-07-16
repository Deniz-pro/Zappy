##
## EPITECH PROJECT, 2025
## G-YEP-400-PAR-4-1-zappy-2
## File description:
## PRO_ForkIA.py
##

from src.Zappy_ai.Utils.find_ressources import scan_resources, coords_cost_need
from src.Zappy_ai.Utils.choice_ressources import pick_priority_target_v2
import random

FOOD_FORK_THRESHOLD  = 12   # food minimum pour forker
FOOD_CRITICAL        = 8   # en dessous -> survie prioritaire
FOOD_SAFE            = 10   # seuil pour reprendre le fork après survie
EXPLORE_MOVES       = ["Forward", "Forward", "Forward", "Right", "Left"]

"""
    PRO_ForkIA — Agent de soutien logistique
    ======================================
    Rôle : augmenter l'effectif de l'équipe par le Fork et neutraliser
        les joueurs adverses présents sur sa tile par Eject.

    Comportement :
    - Ramasser tout les foods qu'il y a sous ses pieds.
        Dès qu'il reçoit le résultat d'un Look, il regarde ce qui se trouve sur
        sa position actuelle et met en file d'attente un Take pour chaque
        nourriture Ces prises sont effectuées avant toute autre action.
    - Survie d'abord : si food < 10, cherche de la nourriture immédiatement
    - Éjection : dès qu'un joueur ennemi est détecté sur sa tile (tile 0
        du résultat Look), envoie Eject et se bloque jusqu'au résultat.
        Un joueur ennemi est tout item de la forme "player-X" où X != team,
        ou "player" seul (sans suffixe d'équipe)
    - Fork : dès que food >= 20 et que la tile est propre (pas d'ennemi),
        envoie Fork et se bloque pendant toute la durée (42/f secondes).
        Aucune commande n'est envoyée pendant ce temps
    - Collecte de food : si food < 20, cherche de la nourriture pour
        atteindre le seuil avant de forker

    Ce que PRO_ForkIA ne fait PAS :
    - Il ne monte jamais de niveau (level figé à 1)
    - Il ne collecte jamais de pierres
    - Il ne lance jamais Incantation
"""

class PRO_ForkIA:
    def __init__(self, team: str):
        self.team             = team
        self.inventory        = {}
        self.the_map          = {}
        self.visible          = {}
        self.objectif         = None
        self.objectif_coords  = None
        self.nbr_move         = 0
        self.nb_steps         = 0
        self.is_in_elev       = False
        self.after_underway   = False
        self.param_is_change  = False
        self.next_command     = None
        self.is_forking       = False
        self.is_ejecting      = False
        self.enemy_on_tile    = False
        self._pending_takes   : list[str] = []
        self._waiting_look    = False
        self.nbr_fork = 0
        self.i_launched_incantation = False
        self.total_forks = 0
        self.want_switch = False
        self.level = 1

    def _explore(self) -> str:
        """Mouvement d'exploration aléatoire, biaisé vers Forward."""
        return random.choice(EXPLORE_MOVES)


    def handle_command(self, command: str, result):

        if command == "Inventory":
            self.inventory       = result
            self.param_is_change = True

        elif command == "Look":
            self._waiting_look = False
            self._queue_current_tile_takes()
            self.the_map         = result
            self.param_is_change = True
            # détecte si un ennemi est sur la tile du joueur (tile 0)
            tile0 = self.the_map.get(0, [])
            nbr_player = 0
            for item in tile0 :
                if item.startswith("player") and not item == f"player-{self.team}" :
                    nbr_player += 1
            if nbr_player > 2 :
                self.enemy_on_tile = True

        elif command == "Fork":
            self.is_forking = False
            self.total_forks += 1
            self.next_command = "Look"

            if self.total_forks >= 15:
                self.want_switch = True

        elif command == "Eject":
            self.is_ejecting  = False
            self.next_command = "Look"

    def _queue_current_tile_takes(self):
        if not self.the_map:
            return
        current_tile: list = self.the_map.get(0, [])
        takes = []
        for item in current_tile:
            if item == "food":
                takes.append("Take " + item)
        self._pending_takes = takes + self._pending_takes


    def add_level(self):
        self.level += 1

    def get_state(self):
        return self.is_in_elev

    def set_state(self, state : bool):
        self.is_in_elev = False

    def reset_all(self):
        self.the_map          = {}
        self.inventory        = {}
        self.is_in_elev       = False
        self._pending_takes   = []
        self._waiting_look    = False
        self.visible         = {}
        self.objectif        = None
        self.objectif_coords = None
        self.nb_steps         = 0
        self.nbr_move        = 0
        self.param_is_change = False

    def handle_broadcast(self, direction: int, text: str):
        return None

    def take_decision(self) -> str | None:

        # --- quota de fork atteint
        if self.want_switch == True :
            return None

        # --- bloqué pendant Fork ou Eject (on attend le résultat) ---
        if self.is_forking or self.is_ejecting:
            return None

        # --- commande injectée (Look post-fork / post-eject) ---
        if self.next_command is not None:
            cmd = self.next_command
            self.next_command = None
            if cmd == "Look" :
                self.the_map = None
            return cmd

        # --- initialisation : récupérer l'état de départ ---
        if self.nb_steps == 0:
            self.nb_steps += 1
            return "Inventory"
        if self.nb_steps == 1:
            self.nb_steps += 1
            return "Look"

        # --- données pas encore reçues ---
        if not self.inventory or not self.the_map:
            return None

        food = self.inventory.get("food", 0)

        # priorité 1 : vider les Take sur la tile courante
        if self._pending_takes:
            return self._pending_takes.pop(0)


        # --- priorité 2 : survie (food critique) ---
        food = self.inventory.get("food", 0)
        if food < FOOD_CRITICAL and self.objectif is not None:
            if self.objectif[0] != "food" :
                self.visible = scan_resources(self.the_map)
                food_info = self.visible.get("food")
                if food_info is not None:
                    # aller chercher la food la plus proche
                    self.objectif        = ("food", food_info[0], food_info[1])
                    self.objectif_coords = (food_info[0], food_info[1], "Take food")
                else:
                    # pas de food visible -> explorer
                    self.reset_all()
                    return "Forward"

        # --- priorité 3 : éjecter les ennemis sur notre tile ---
        elif self.enemy_on_tile:
            self.is_ejecting   = True
            self.enemy_on_tile = False
            return "Eject"

        # --- priorité 4 : forker si assez de food et tile propre ---
        elif food >= FOOD_FORK_THRESHOLD and not self.enemy_on_tile:
            if self.nbr_fork < 2 :
                self.is_forking = True
                self.nbr_fork += 1
                return "Fork"
            else :
                self.nbr_fork = 0
                return "Inventory"

        # --- priorité 5 : collecter de la food pour atteindre le seuil ---
        # --- priorité 4 : recalcul objectif si état changé
        elif self.param_is_change:
            self.param_is_change = False
            self.nbr_move        = 0
            self.visible         = scan_resources(look_result=self.the_map)
            food_info = self.visible.get("food")
            if food_info is not None:
                # aller chercher la food la plus proche
                self.objectif        = ("food", food_info[0], food_info[1])
                self.objectif_coords = (food_info[0], food_info[1], "Take food")

        # pas d'objectif : avance d'un pas puis demande un Look
        # — on ne lance le Look que si aucun n'est déjà en vol
        if self.objectif is None:
            if not self._waiting_look:
                self._waiting_look = True
                self.next_command  = "Look"
                self.the_map = {}
            return self._explore()


        # --- déplacement vers objectif ---
        if self.objectif_coords is not None:
            if self.nbr_move < self.objectif_coords[1]:
                self.nbr_move += 1
                return "Forward"

            if self.nbr_move == self.objectif_coords[1]:
                ressource = self.objectif[0]
                self.reset_all()
                # rafraîchir inventaire après ramassage
                self.next_command = "Inventory"
                return "Take " + ressource

        # --- fallback : explorer et rafraîchir ---
        self.next_command = "Look"
        return "Forward"