from src.Zappy_ai.Utils.elevation import missing_stones
from src.Zappy_ai.Utils.find_ressources import moves_to_tile

FOOD_THRESHOLD = 20

"""
    Cette fonction renvoie la ressources choisie selon des conditions et les infortmations sont
    envoyés la forme (ressources, index, nombre de coups pour atteindre la ressources)
"""
def pick_priority_target(inventory: dict, visible: dict, level: int) -> tuple[str, int, int] | None:
    food_info = visible.get("food")

    if inventory.get("food", 0) < FOOD_THRESHOLD and food_info is not None:
        return ("food", food_info[0], food_info[1])

    manquantes = missing_stones(level, inventory)

    cibles = {
        r: visible[r]
        for r in manquantes
        if visible.get(r) is not None
    }

    if cibles:
        meilleure = min(
            cibles.items(),
            key=lambda item: (inventory.get(item[0], 0), item[1][1])
        )
        ressource, (tile_index, nb_coups) = meilleure
        return (ressource, tile_index, nb_coups)

    if food_info is not None:
        return ("food", food_info[0], food_info[1])

    return None

def pick_priority_target_v2(
    inventory: dict,
    visible: dict,
    look_result: dict,
    level: int
) -> tuple[str, int, int] | None:

    # --- cible prioritaire via l'ancienne logique ---
    priority = pick_priority_target(inventory, visible, level)

    # --- meilleure tile multi-ressources ---
    best_tile_index = None
    best_score      = float('inf')  # on veut minimiser (coût - quantité)

    for tile_index, contents in look_result.items():
        if tile_index == 0:
            continue
        if not contents:
            continue

        qty = 0
        for res in contents :
            if res != "player" :
                qty += 1
        cost  = moves_to_tile(tile_index)

        if qty == 0:
            continue

        score = cost - qty  # plus c'est bas, plus la tile est rentable
        if score < best_score:
            best_score      = score
            best_tile_index = tile_index

    # --- aucune tile trouvée : retombe sur la priorité de base ---
    if best_tile_index is None:
        return priority

    # --- aucune cible prioritaire : prend la meilleure tile ---
    if priority is None:
        resource = next(
            (k for k in look_result[best_tile_index] if k != "player"),
            None
        )
        if resource is None:
            return None
        return (resource, best_tile_index, moves_to_tile(best_tile_index))

    # --- les deux existent : prend le plus proche ---
    priority_cost   = priority[2]
    best_tile_cost  = moves_to_tile(best_tile_index)

    if best_tile_cost < priority_cost:
        # la tile multi-ressources est plus proche
        # on retourne la ressource la plus présente sur cette tile
        resource = max(
            ((k) for k in look_result[best_tile_index] if k != "player"),
            key=lambda item: item[1]
        )[0]
        return (resource, best_tile_index, best_tile_cost)

    return priority