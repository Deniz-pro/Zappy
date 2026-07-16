def parse_look(raw: str) -> dict:
    raw = raw.strip().lstrip("[").rstrip("]")
    tiles = raw.split(",")
    result = {}
    for i, tile in enumerate(tiles):
        tile = tile.strip()
        result[i] = [item for item in tile.split(" ") if item]
    return result

def parse_inventory(raw: str) -> dict:
    raw = raw.strip().lstrip("[").rstrip("]")
    result = {}
    for entry in raw.split(","):
        parts = entry.strip().split(" ")
        if len(parts) != 2:
            return {}
        name, qty = parts[0].strip(), parts[1].strip()
        try:
            result[name] = int(qty)
        except ValueError:
            return {}
    return result

def parse_incantation(raw: str):
    """
    Deux réponses possibles du serveur :
    - "Current level: k"  -> incantation réussie, retourne le nouveau niveau (int)
    - "ko"                -> incantation échouée, retourne None

    Note : le serveur envoie d'abord "Elevation underway" quand la commande
    est acceptée, puis "Current level: k" ou "ko" quand c'est terminé.
    Ce parser est appelé deux fois — une fois pour chaque ligne.
    """
    val = raw.strip().lower()

    if val == "ko":
        return None

    if val.startswith("current level:"):
        try:
            return int(val.split(":")[1].strip())
        except ValueError:
            return None

    # "elevation underway" -> incantation en cours, pas encore terminée
    if "elevation underway" in val:
        return "underway"

    return None