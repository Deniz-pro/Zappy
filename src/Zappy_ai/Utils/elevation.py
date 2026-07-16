from dataclasses import dataclass
from typing import Optional


@dataclass
class ElevationReq:
    level:     int
    players:   int
    linemate:  int
    deraumere: int
    sibur:     int
    mendiane:  int
    phiras:    int
    thystame:  int

ELEVATION_TABLE: list[ElevationReq] = [
    ElevationReq(1, players=1, linemate=1, deraumere=0, sibur=0, mendiane=0, phiras=0, thystame=0),
    ElevationReq(2, players=2, linemate=1, deraumere=1, sibur=1, mendiane=0, phiras=0, thystame=0),
    ElevationReq(3, players=2, linemate=2, deraumere=0, sibur=1, mendiane=0, phiras=2, thystame=0),
    ElevationReq(4, players=4, linemate=1, deraumere=1, sibur=2, mendiane=0, phiras=1, thystame=0),
    ElevationReq(5, players=4, linemate=1, deraumere=2, sibur=1, mendiane=3, phiras=0, thystame=0),
    ElevationReq(6, players=6, linemate=1, deraumere=2, sibur=3, mendiane=0, phiras=1, thystame=0),
    ElevationReq(7, players=6, linemate=2, deraumere=2, sibur=2, mendiane=2, phiras=2, thystame=1),
]


def get_req(level: int) -> Optional[ElevationReq]:
    if level < 1 or level > 7:
        return None
    return ELEVATION_TABLE[level - 1]


def missing_stones(level: int, inventory: dict) -> dict:
    """
    Fonction qui renvoie un dictionnaire de la quantité de chaque type 
    de ressources pour atteindre le niveau envoyé , en se servant de
    l'inventaire du joueur , le résultat est envoyé dans un dictionnaire python 
    """
    req = get_req(level)
    if req is None:
        return {}
    missing = {}
    for stone in ["linemate", "deraumere", "sibur", "mendiane", "phiras", "thystame"]:
        needed  = getattr(req, stone)
        have    = inventory.get(stone, 0)
        if have < needed:
            missing[stone] = needed - have
    return missing
