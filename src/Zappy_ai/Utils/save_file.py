import json
import atexit

class StatsTracker:
    def __init__(self, output_path: str = "zappy_stats.json"):
        self.output_path  = output_path
        self.total        = 0
        self.commands     = {}
        self.max_level    = 1
        self.final_inv    = {}
        self.max_inv      = {}
        atexit.register(self.save)

    def update(self, command: str, ia, level):
        base = command.split()[0]
        self.total += 1
        self.commands[base] = self.commands.get(base, 0) + 1
        self.max_level = level
        if ia.inventory:
            self.final_inv = dict(ia.inventory)
            for k, v in ia.inventory.items():
                self.max_inv[k] = max(self.max_inv.get(k, 0), v)
        self.save()

    def save(self):
        with open(self.output_path, "w") as f:
            json.dump({
                "total_commands":  self.total,
                "commands":        self.commands,
                "max_level":       self.max_level,
                "final_inventory": self.final_inv,
                "max_inventory":   self.max_inv,
            }, f, indent=2)

