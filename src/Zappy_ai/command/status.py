from enum import Enum
import sys


class Response(Enum):
    NOTHING = 0
    OK      = 1
    KO      = 2

def parse_ok(raw: str) -> Response:
    _ = raw
    return Response.OK

def parse_ok_ko(raw: str) -> Response:
    val = raw.strip().lower()
    if val == "ok":
        return Response.OK
    if val == "ko":
        return Response.KO
    return Response.NOTHING

def parse_connect_nbr(raw: str) -> int:
    try:
        return int(raw.strip())
    except ValueError:
        return -1

def handle_dead(raw: str) -> None:
    _ = raw
    print("[dead] player starved", file=sys.stderr)
    sys.exit(0)
