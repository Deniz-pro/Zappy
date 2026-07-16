##
## EPITECH PROJECT, 2025
## G-YEP-400-PAR-4-1-zappy-2
## File description:
## PRO_IA.py
##

from src.Zappy_ai.Utils.choice_ressources import pick_priority_target_v2
from src.Zappy_ai.Utils.find_ressources import scan_resources, coords_cost_need
from src.Zappy_ai.Utils.elevation import missing_stones

Command_need_handle = ["Inventory", "Look", "Connect_nbr"]
FOOD_CRITICAL       = 10   # en dessous -> survie prioritaire
FOOD_MIN_ELEV       = 20


"""
    PRO_IA — Agent d'élévation solo
    ==================================

    Rôle
    ----
    PRO_IA est un agent autonome dont l'unique objectif est de monter de niveau
    le plus vite possible, du niveau 1 au niveau 8, sans jamais interagir avec
    les autres joueurs.

    Comment il fonctionne
    ----------------------
    À chaque fois qu'on lui demande une décision, l'agent suit toujours le même
    ordre de priorité :

    1. Ramasser ce qu'il y a sous ses pieds.
        Dès qu'il reçoit le résultat d'un Look, il regarde ce qui se trouve sur
        sa position actuelle et met en file d'attente un Take pour chaque
        ressource présente (nourriture, pierres — tout sans exception).
        Ces prises sont effectuées avant toute autre action.

    2. Monter de niveau si possible.
        Si l'inventaire contient toutes les pierres nécessaires au prochain
        rituel et au moins 4 unités de nourriture, il lance immédiatement
        l'incantation et attend son résultat sans rien faire d'autre.

    3. Se diriger vers une ressource visible.
        Si une ressource intéressante est visible sur le terrain, il calcule
        le chemin le plus court pour l'atteindre et avance pas à pas vers elle.
        Une fois arrivé, il la ramasse et repart de zéro.

    4. Explorer si rien n'est visible.
        Quand aucune ressource n'est en vue, il avance d'un pas et demande
        un nouveau Look. Pour éviter de saturer le serveur, il ne relance
        jamais un Look tant que le précédent n'a pas reçu de réponse.

    Ce que PRO_IA ne fait pas
    ---------------------------
    Il ne forke jamais, n'éjecte jamais personne et ne communique pas
    via Broadcast. Il joue entièrement en solo.
"""


class PRO_IA:

    def __init__(self, team: str):
        self.team             = team
        self.the_map          = {}
        self.inventory        = {}
        self.visible          = {}
        self.objectif         = None
        self.objectif_coords  : tuple = None
        self.param_is_change  = False
        self.is_in_elev       = False
        self.after_underway   = False
        self.nbr_move         = 0
        self.nb_steps         = 0
        self.Connect_nbr      = 0
        self.level            = 1
        self.next_command     = None
        self._pending_takes   : list[str] = []
        self._waiting_look    = False
        self.nbr_of_incantation = 0
        self.skip_incantation = 0

    def handle_command(self, command: str, result):
        if command in Command_need_handle:
            self.param_is_change = True
        if command == "Inventory":
            self.inventory = result
        elif command == "Look":
            self._waiting_look = False
            self.the_map = result
            self._queue_current_tile_takes()
        elif command == "Connect_nbr":
            self.Connect_nbr = result
        elif command == "Incantation":
            if result == "underway":
                self.after_underway = True
                pass
            elif result is None:
                self.is_in_elev = False
                self.nbr_of_incantation += 1
                self.reset_all()
            elif isinstance(result, int):
                self.level = result
                self.is_in_elev = False
                self.reset_all()

    def _queue_current_tile_takes(self):
        if not self.the_map:
            return
        current_tile: list = self.the_map.get(0, [])
        takes = []
        for item in current_tile:
            if item == "player" or item.startswith("player-"):
                continue
            takes.append("Take " + item)
        takes.append("Inventory")
        self._pending_takes = takes + self._pending_takes

    def add_level(self):
        self.level += 1

    def get_state(self):
        return self.is_in_elev

    def set_state(self, state: bool):
        self.is_in_elev = state

    def reset_all(self):
        self.the_map          = {}
        self.inventory        = {}
        self.visible          = {}
        self.objectif         = None
        self.nb_steps         = 0
        self.objectif_coords  = None
        self.is_in_elev       = False
        self.nbr_move         = 0
        self.param_is_change  = False
        self._pending_takes   = []
        self._waiting_look    = False

    def take_decision(self) -> str | None:

        print(f"""
        ===== ÉTAT DU JOUEUR =====
        Map               : {self.the_map}
        Inventaire        : {self.inventory}
        Visible           : {self.visible}
        
        Objectif          : {self.objectif}
        Coord. objectif   : {self.objectif_coords}
        
        Nombre de steps   : {self.nb_steps}
        Nombre de moves   : {self.nbr_move}
        
        Dans ascenseur    : {self.is_in_elev}
        Param changé      : {self.param_is_change}
        
        Pending takes     : {self._pending_takes}
        Waiting look      : {self._waiting_look}
        =========================
        """)
        # bloqué pendant incantation
        if self.is_in_elev:
            return None
        
        if self.skip_incantation > 5 :
            self.nbr_of_incantation = 0
            self.skip_incantation = 0

        # commande injectée
        if self.next_command is not None:
            command = self.next_command
            self.next_command = None
            return command

        # initialisation
        if self.nb_steps == 0:
            self.nb_steps += 1
            return "Inventory"
        if self.nb_steps == 1:
            self.nb_steps += 1
            return "Look"

        # données pas encore reçues
        if self.inventory == {} or self.the_map == {}:
            return None

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
                    print("en maque de food , pas de food en vue ")
                    return "Forward"

        # --- priorité 3 : élévation
        need_stones = missing_stones(level=self.level, inventory=self.inventory)
        if sum(need_stones.values()) == 0 and self.inventory.get("food", 0) >= 4 and food > FOOD_MIN_ELEV:
            if self.nbr_of_incantation < 5 :
                self.is_in_elev = True
                return "Incantation"
            else :
                self.skip_incantation += 1

        # --- priorité 4 : recalcul objectif si état changé
        if self.param_is_change:
            self.param_is_change = False
            self.nbr_move        = 0
            self.visible         = scan_resources(look_result=self.the_map)
            self.objectif        = pick_priority_target_v2(
                inventory=self.inventory,
                visible=self.visible,
                level=self.level,
                look_result=self.the_map
            )
            if self.objectif is not None:
                self.objectif_coords = coords_cost_need(self.objectif[1])

        # pas d'objectif : avance d'un pas puis demande un Look
        # — on ne lance le Look que si aucun n'est déjà en vol
        if self.objectif is None:
            if not self._waiting_look:
                self._waiting_look = True
                self.next_command  = "Look"
                self.the_map = {}
            return "Forward"

        # déplacement vers objectif
        if self.nbr_move < self.objectif_coords[1]:
            self.nbr_move += 1
            return "Forward"

        # rotation à l'arrivée
        if self.nbr_move == self.objectif_coords[1]:
            self.nbr_move += 1
            return self.objectif_coords[2]

        # ramasser
        if self.nbr_move > self.objectif_coords[1]:
            ressource = self.objectif[0]
            self.reset_all()
            return "Take " + ressource

        return "Forward"