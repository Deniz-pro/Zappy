from src.Zappy_ai.Utils.choice_ressources import pick_priority_target
from src.Zappy_ai.Utils.find_ressources import scan_resources , coords_cost_need
from src.Zappy_ai.Utils.elevation import missing_stones

Command_need_handle = ["Inventory", "Look", "Connect_nbr"]

"""
    BasicIA — Agent d'élévation
    ============================
    Rôle : monter le joueur du niveau 1 au niveau 8 le plus vite possible.

    Comportement :
    - Survie d'abord : si food < 15, cherche de la nourriture en priorité absolue
    - Collecte de pierres : ramasse les pierres manquantes pour le prochain rituel
        d'élévation, en priorisant celles qu'il possède en moins grande quantité
    - Élévation : dès que toutes les pierres sont réunies et food >= 4,
        lance Incantation et se bloque jusqu'au résultat
    - Exploration : si rien de visible, avance et relance un Look

    Ce que BasicIA ne fait PAS :
    - Il ne forke jamais
    - Il n'éjecte jamais personne
    - Il ne communique pas via Broadcast
"""

class BasicIA:

    def __init__(self, team : str):
        self.team = team
        self.the_map          = {}
        self.inventory        = {}
        self.visible          = {}
        self.objectif         = {}
        self.objectif_coords  : tuple = None
        self.param_is_change  = False
        self.is_in_elev       = False
        self.nbr_move         = 0
        self.nb_steps         = 0
        self.Connect_nbr      = 0
        self.level            = 1
        self.next_command = None

    def handle_command(self, command: str, result):
        if command in Command_need_handle:
            self.param_is_change = True
        if command == "Inventory":
            self.inventory = result
        elif command == "Look":
            self.the_map = result
        elif command == "Connect_nbr":
            self.Connect_nbr = result
        elif command == "Incantation":
            if result == "underway":
                # serveur confirme le début -> on reste bloqué, rien à faire
                pass
            elif result is None:
                # ko -> incantation échouée, on repart propre
                self.is_in_elev = False
                self.reset_all()
            elif isinstance(result, int):
                # succès -> nouveau niveau reçu
                self.level = result
                self.is_in_elev = False
                self.reset_all()

    def add_level(self):
        self.level += 1

    def get_state(self):
        return self.is_in_elev

    def set_state(self, state : bool):
        self.is_in_elev = state

    def reset_all(self):
        self.the_map          = {}
        self.inventory        = {}
        self.visible          = {}
        self.objectif         = {}
        self.nb_steps         = 0
        self.objectif_coords  = None
        self.is_in_elev       = False
        self.nbr_move         = 0
        self.param_is_change  = False

    def take_decision(self) -> str | None:
        # --- bloqué pendant incantation ---
        if self.is_in_elev:
            return None

        # --- commande injectée (Look post-fork / post-eject) ---
        if self.next_command is not None :
            command = self.next_command
            self.next_command = None
            return command

        # --- initialisation : récupérer l'état de départ ---
        if self.nb_steps == 0:
            self.nb_steps += 1
            return "Inventory"
        if self.nb_steps == 1:
            self.nb_steps += 1
            return "Look"

        # --- données pas encore reçues ---
        if self.inventory == {} or self.the_map == {}:
            return None

        # --- Lancement de l'élévation ---
        need_stones = missing_stones(level=self.level, inventory=self.inventory)
        nbr_ressources = sum(need_stones.values())
        if nbr_ressources == 0 and self.inventory.get("food", 0) >= 4:
            self.is_in_elev = True
            return "Incantation"

        # --- recalcul objectif si état changé ---
        if self.param_is_change:
            self.param_is_change  = False
            self.nbr_move         = 0
            self.visible          = scan_resources(look_result=self.the_map)
            self.objectif         = pick_priority_target(
                inventory=self.inventory,
                visible=self.visible,
                level=self.level
            )
            if self.objectif == None :
                self.next_command = "Look"
                return "Forward"
            self.objectif_coords  = coords_cost_need(self.objectif[1])

        # --- vérifie si il y a déjà un objectif ---
        if self.objectif == None :
            self.next_command = "Look"
            return "Forward"

        # --- déplacement vers objectif ---
        if self.nbr_move < self.objectif_coords[1]:
            self.nbr_move += 1
            return "Forward"

        # --- arrivé sur la tile -> tourner de direction ---
        if self.nbr_move == self.objectif_coords[1]:
            self.nbr_move += 1
            return self.objectif_coords[2]

        # --- ramasser -> reset et recommence ---
        if self.nbr_move > self.objectif_coords[1]:
            ressource = self.objectif[0]        # sauvegarder avant reset
            self.reset_all()
            return "Take " + ressource

        return "Forward"