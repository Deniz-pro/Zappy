import socket
import select
import sys
from collections import deque
from src.Zappy_ai.Utils.stats_viewer import viewer


class SocketManager:
    def __init__(self, host: str, port: int):
        self.host = host
        self.port = port
        self.sock = None
        self.send_buffer = ""
        self.recv_buffer = ""
        self.pending = deque()
        self.pending_commands = []
        self.file = None

    def connect(self):
        """Ouvre la connexion TCP vers le serveur."""
        try:
            self.sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
            self.sock.connect((self.host, self.port))
            self.sock.setblocking(False)
        except (socket.error, ConnectionRefusedError) as e:
            print(f"Connection failed: {e}", file=sys.stderr)
            sys.exit(84)

    def queue_command(self, command: str, callback):
        """Ajoute une commande au send_buffer et son callback en pending."""
        self.send_buffer += command + "\n"
        self.pending_commands.append(command)
        self.pending.append(callback)
    
    def flush_send(self):
        """Envoie ce qui est dans le send_buffer si le socket est prêt."""
        if not self.send_buffer:
            return
        try:
            sent = self.sock.send(self.send_buffer.encode())
            self.send_buffer = self.send_buffer[sent:]
        except BlockingIOError:
            pass
        except socket.error as e:
            print(f"Send error: {e}", file=sys.stderr)
            sys.exit(84)

    def read_available(self):
        """Lit ce qui est disponible et retourne les lignes complètes."""
        try:
            data = self.sock.recv(4096).decode()
            if not data:
                print("Server closed connection", file=sys.stderr)
                if self.file != None :
                    viewer(self.file)
                sys.exit(0)
            self.recv_buffer += data
        except BlockingIOError:
            return []
        except socket.error as e:
            print(f"Recv error: {e}", file=sys.stderr)
            sys.exit(84)

        lines = self.recv_buffer.split("\n")
        self.recv_buffer = lines[-1]
        return [l for l in lines[:-1] if l]

    def tick(self, timeout=0.05):
        """
        Un tour de boucle : select() -> flush send -> dispatch réponses.
        Retourne les lignes reçues non consommées par le handshake.
        """
        reads, writes, _ = select.select(
            [self.sock],
            [self.sock] if self.send_buffer else [],
            [],
            timeout
        )
        if writes:
            self.flush_send()

        lines = []
        if reads:
            lines = self.read_available()

        return lines

    def dispatch(self, lines: list):
        """
        Pour chaque ligne reçue, appelle le callback FIFO correspondant.
        Les messages non sollicités (dead, eject, message) sont gérés séparément.
        """
        for line in lines:
            if line.startswith("dead"):
                print("dead", file=sys.stderr)
                if self.file != None :
                    viewer(self.file)
                sys.exit(0)
            if line.startswith("message") or line.startswith("eject:"):
                # après il vas falloir gérer broadcast / éjection
                continue
            if self.pending:
                cb = self.pending.popleft()
                self.pending_commands.pop(0)
                cb(line)
