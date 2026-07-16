import sys


class Handshake:
    """
    Gère la séquence d'initialisation :
        - WELCOME
        - TEAM-NAME
        - CLIENT-NUM
        - X Y
    """

    def __init__(self, sock_manager, team_name: str):
        self.sock = sock_manager
        self.team_name = team_name
        self.client_num = None
        self.world_x = None
        self.world_y = None
        self._step = 0

    def process(self, line: str) -> bool:
        """
        Avance la machine à états du handshake.
        Retourne True quand le handshake est terminé.
        """
        if self._step == 0:
            if line.strip() != "WELCOME":
                print(f"Expected WELCOME, got: {line!r}", file=sys.stderr)
                sys.exit(84)
            self.sock.sock.send((self.team_name + "\n").encode())
            self._step = 1

        elif self._step == 1:
            try:
                self.client_num = int(line.strip())
            except ValueError:
                print(f"Expected CLIENT-NUM, got: {line!r}", file=sys.stderr)
                sys.exit(84)
            self._step = 2

        elif self._step == 2:
            parts = line.strip().split()
            if len(parts) != 2:
                print(f"Expected 'X Y', got: {line!r}", file=sys.stderr)
                sys.exit(84)
            try:
                self.world_x, self.world_y = int(parts[0]), int(parts[1])
            except ValueError:
                print(f"Invalid world size: {line!r}", file=sys.stderr)
                sys.exit(84)
            return True

        return False

    @property
    def done(self):
        return self._step == 2 and self.world_x is not None
