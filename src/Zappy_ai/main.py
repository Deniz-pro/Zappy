##
## EPITECH PROJECT, 2025
## G-YEP-400-PAR-4-1-zappy-2
## File description:
## main.py
##

import sys
from src.Zappy_ai.Utils.args import parse_args
from src.Zappy_ai.client.socket_manager import SocketManager
from src.Zappy_ai.client.handshake import Handshake
from src.Zappy_ai.command.parse_command import parse_look, parse_inventory, parse_incantation
from src.Zappy_ai.command.status import parse_ok, parse_ok_ko, parse_connect_nbr
from src.Zappy_ai.AI.PRO_IA import PRO_IA
from src.Zappy_ai.AI.PRO_ForkIA import PRO_ForkIA
from src.Zappy_ai.Utils.save_file import StatsTracker

COMMAND_PARSERS = {
    "Forward":     parse_ok,
    "Right":       parse_ok,
    "Left":        parse_ok,
    "Look":        parse_look,
    "Inventory":   parse_inventory,
    "Broadcast":   parse_ok,
    "Connect_nbr": parse_connect_nbr,
    "Fork":        parse_ok,
    "Eject":       parse_ok_ko,
    "Take":        parse_ok_ko,
    "Set":         parse_ok_ko,
    "Incantation": parse_incantation,
}

# États de la machine à états
STATE_INIT          = "init"
STATE_COLLECT       = "collect"       # collecte food + pierres
STATE_SEEK_PARTNER  = "seek_partner"  # cherche un player visible
STATE_GOTO_RENDEZ   = "goto_rendez"   # se dirige vers le point de rendez-vous
STATE_WAIT_PARTNER  = "wait_partner"  # attend au point de rendez-vous
STATE_BROADCAST     = "broadcast"     # échange de broadcast pour vérifier level
STATE_DROP_STONES   = "drop_stones"   # dépose les pierres au sol
STATE_INCANTATION   = "incantation"   # lance l'incantation



def parse_broadcast_event(line: str) -> tuple[int, str] | None:
    """
    Parse une ligne 'message K, text' envoyée spontanément par le serveur.
    Retourne (direction, text) ou None si ce n'est pas un broadcast.
    """
    line = line.strip()
    if not line.startswith("message "):
        return None
    try:
        # "message K, text"
        rest      = line[len("message "):]
        comma_idx = rest.index(",")
        direction = int(rest[:comma_idx].strip())
        text      = rest[comma_idx + 1:].strip()
        return (direction, text)
    except (ValueError, IndexError):
        return None


def make_callback(ia, command: str):
    for cmd in COMMAND_PARSERS:
        if command.startswith(cmd):
            parser = COMMAND_PARSERS.get(cmd)
    def callback(raw: str):
        if parser is None:
            return
        result = parser(raw)
        ia.handle_command(command, result)
    return callback


def pick_ia(team: str, connect_nbr: int):
    if 4 <= connect_nbr <= 5:
        print(f"[IA] PRO_ForkIA sélectionnée (slots={connect_nbr})")
        return PRO_ForkIA(team)
    print(f"[IA] PRO_IA sélectionnée (slots={connect_nbr})")
    return PRO_IA(team)


def main():
    args = parse_args()
    sock = SocketManager(args.h, args.p)
    sock.connect()
    hs           = Handshake(sock, args.n)
    handshake_done = False
    line_to_remove = None

    # phase handshake
    while not handshake_done:
        lines = sock.tick(timeout=2.0)
        if not lines:
            print("Timeout waiting for server", file=sys.stderr)
            sys.exit(84)
        for line in lines:
            if hs.process(line):
                handshake_done = True
                break

    print(f"[+] Connected | team={args.n} | slots={hs.client_num} | map={hs.world_x}x{hs.world_y}")

    # lancement de la commande connect_nbr
    nbr_connect_result = []

    def store_connect(raw: str):
        nbr_connect_result.append(parse_connect_nbr(raw))

    sock.queue_command("Connect_nbr", store_connect)

    while not nbr_connect_result:
        lines = sock.tick()
        if lines:
            sock.dispatch(lines)

    nbr_connect = nbr_connect_result[0]
    ia          = pick_ia(args.n, nbr_connect)

    # boucle principale
    pending_look = False
    file = "Zappy_ia_stats_" + args.n + str(nbr_connect_result[0]) + ".json"
    sock.file = file
    tracker = StatsTracker(file)

    while True:
        if len(sock.pending) < 10:
            if pending_look and len(sock.pending) == 0:
                pending_look = False
                sock.queue_command("Look", make_callback(ia, "Look"))
                tracker.update("Look", ia, ia.level)
            else:
                command = ia.take_decision()
                if command is not None:
                    if command == "Look":
                        pending_look = True
                    else:
                        sock.queue_command(command, make_callback(ia, command))
                        tracker.update(command, ia, ia.level)

        lines = sock.tick()


        if line_to_remove is not None:
            if line_to_remove in lines:
                lines.remove(line_to_remove)
            line_to_remove = None

        # --- traitement des lignes avant dispatch ---
        lines_to_dispatch = []
        for line in lines:

            # 1. broadcasts entrants (événement spontané, pas une réponse à une commande)
            broadcast = parse_broadcast_event(line)
            if broadcast is not None:
                direction, text = broadcast
                ia.handle_broadcast(direction, text)
                # on ne dispatche pas cette ligne, elle n'est pas dans la queue
                continue

            # 2. événements d'élévation (réponses tardives de l'incantation)
            if line.startswith("Elevation underway"):
                ia.is_in_elev     = True
                ia.after_underway = True
                ia.state          = STATE_INCANTATION
                line_to_remove    = line
                # Supprimer uniquement le callback Incantation s'il est dans la queue
                for i, cmd in enumerate(sock.pending_commands):
                        if cmd.startswith("Incantation"):
                            del sock.pending_commands[i]
                            del sock.pending[i]
                            break
                continue

            if ia.is_in_elev and ia.after_underway:
                if line.startswith("Current level:"):
                    ia.add_level()
                    ia.after_underway = False
                    ia.is_in_elev     = False
                    ia._go_collect()
                    line_to_remove = line
                    continue
                if line.startswith("ko"):
                    ia.after_underway = False
                    ia.is_in_elev     = False
                    ia._go_collect()
                    line_to_remove    = line
                    continue

            if sock.pending_commands and sock.pending_commands[0] == "Inventory":
                if not line.startswith("[ food "):
                    continue

            if sock.pending_commands and sock.pending_commands[0] == "Look":
                if not line.startswith("[ player") :
                    continue
    
            lines_to_dispatch.append(line)

        if lines_to_dispatch:
            sock.dispatch(lines_to_dispatch)
        if (
            isinstance(ia, PRO_ForkIA)
            and ia.want_switch
            and len(sock.pending) == 0
        ):
            print("[IA] Passage de PRO_ForkIA -> PRO_IA")
            ia = PRO_IA(args.n)


if __name__ == "__main__":
    main()