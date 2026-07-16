
RESOURCES = ["food", "linemate", "deraumere", "sibur", "mendiane", "phiras", "thystame"]

def tile_index_to_coords(tile_index: int) -> tuple[int, int]:
    if tile_index == 0:
        return (0, 0)

    row = int(tile_index ** 0.5)
    if row * row > tile_index:
        row -= 1

    line_start = row * row
    col = tile_index - line_start - row

    return (col, row)


def coords_cost_need(tile_index: int) -> tuple[int , int , str] :
    col , row  = tile_index_to_coords(tile_index=tile_index)
    if col < 0 : 
        col = - col
        move =  "Left"
    else :
        move = "Right"
    return (col, row , move)

def moves_to_tile(tile_index: int) -> int:
    col, row = tile_index_to_coords(tile_index)
    lateral_moves = abs(col) * 2 if col != 0 else 0
    return row + lateral_moves


def nearest_tile_with(look_result: dict, target: str) -> tuple[int, int] | None:
    best_index = None
    best_cost  = float('inf')

    for tile_index, contents in look_result.items():
        if tile_index == 0:
            continue
        if target in contents:
            cost = moves_to_tile(tile_index)
            if cost < best_cost:
                best_cost  = cost
                best_index = tile_index

    if best_index is None:
        return None
    return (best_index, best_cost)

def scan_resources(look_result: dict) -> dict:

    return {
        resource: nearest_tile_with(look_result, resource)
        for resource in RESOURCES
    }