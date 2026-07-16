from src.Zappy_ai.Utils.find_ressources import scan_resources
from src.Zappy_ai.Utils.choice_ressources import pick_priority_target

FOOD_FORK_THRESHOLD  = 20   # food minimum pour forker
FOOD_CRITICAL        = 10   # en dessous -> survie prioritaire
FOOD_SAFE            = 15   # seuil pour reprendre le fork après survie

"""
    ForkIA — Agent de soutien logistique
    ======================================
    Rôle : augmenter l'effectif de l'équipe par le Fork et neutraliser
        les joueurs adverses présents sur sa tile par Eject.

    Comportement :
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

    Ce que ForkIA ne fait PAS :
    - Il ne monte jamais de niveau (level figé à 1)
    - Il ne collecte jamais de pierres
    - Il ne lance jamais Incantation
"""

class ForkIA:
    def __init__(self, team: str):
        self.team             = team
        self.inventory        = {}
        self.the_map          = {}
        self.visible          = {}
        self.objectif         = None
        self.objectif_coords  = None
        self.nbr_move         = 0
        self.nb_steps         = 0
        self.param_is_change  = False
        self.next_command     = None
        self.is_forking       = False
        self.is_ejecting      = False
        self.enemy_on_tile    = False

    def handle_command(self, command: str, result):
        if command == "Inventory":
            self.inventory       = result
            self.param_is_change = True

        elif command == "Look":
            self.the_map         = result
            self.param_is_change = True
            # détecte si un ennemi est sur la tile du joueur (tile 0)
            tile0 = self.the_map.get(0, [])
            self.enemy_on_tile = any(
                item.startswith("player") and not item == f"player-{self.team}"
                for item in tile0
            )

        elif command == "Fork":
            # fork terminé -> on repart
            self.is_forking = False
            self.next_command = "Look"

        elif command == "Eject":
            self.is_ejecting  = False
            self.next_command = "Look"

    def add_level(self):
        self.level += 1

    def get_state(self):
        return self.is_in_elev

    def set_state(self, state : bool):
        self.is_in_elev = False

    def reset_all(self):
        self.visible         = {}
        self.objectif        = None
        self.objectif_coords = None
        self.nb_steps         = 0
        self.nbr_move        = 0
        self.param_is_change = False

    def take_decision(self) -> str | None:

        # --- bloqué pendant Fork ou Eject (on attend le résultat) ---
        if self.is_forking or self.is_ejecting:
            return None

        # --- commande injectée (Look post-fork / post-eject) ---
        if self.next_command is not None:
            cmd = self.next_command
            self.next_command = None
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
            return "Inventory"

        food = self.inventory.get("food", 0)

        # --- priorité 1 : survie (food critique) ---
        if food < FOOD_CRITICAL:
            self.reset_all()
            self.visible = scan_resources(self.the_map)
            food_info = self.visible.get("food")
            if food_info is not None:
                # aller chercher la food la plus proche
                self.objectif        = ("food", food_info[0], food_info[1])
                self.objectif_coords = (food_info[0], food_info[1], "Take food")
            else:
                # pas de food visible -> explorer
                return "Forward"

        # --- priorité 2 : éjecter les ennemis sur notre tile ---
        elif self.enemy_on_tile:
            self.is_ejecting   = True
            self.enemy_on_tile = False
            return "Eject"

        # --- priorité 3 : forker si assez de food et tile propre ---
        elif food >= FOOD_FORK_THRESHOLD and not self.enemy_on_tile:
            self.is_forking = True
            return "Fork"

        # --- priorité 4 : collecter de la food pour atteindre le seuil ---
        else:
            if self.param_is_change:
                self.reset_all()
                self.visible  = scan_resources(self.the_map)
                self.objectif = pick_priority_target(
                    inventory=self.inventory,
                    visible=self.visible,
                    level=1       # ForkIA ne monte pas de niveau
                )
                if self.objectif is None:
                    # rien de visible -> explorer
                    self.next_command = "Look"
                    return "Forward"
                tile_idx           = self.objectif[1]
                from src.Zappy_ai.Utils.find_ressources import coords_cost_need
                self.objectif_coords = coords_cost_need(tile_idx)
                self.param_is_change = False

        # --- vérifie si il y a déjà un objectif ---
        if self.objectif == None :
            self.next_command = "Look"
            return "Forward"

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