#!/usr/bin/env python3

import json
import sys

from textual.app import App, ComposeResult
from textual.containers import Horizontal, Vertical
from textual.widgets import Header, Footer, Static, DataTable


class Summary(Static):

    def __init__(self, stats):
        super().__init__()
        self.stats = stats

    def on_mount(self):

        self.update(f"""
[bold cyan] ZAPPY AI[/]

[green]Total commands[/]
[bold]{self.stats["total_commands"]}[/]

[yellow]Maximum level[/]
[bold]{self.stats["max_level"]}[/]

[magenta]Different commands[/]
[bold]{len(self.stats["commands"])}[/]
""")



class Inventory(DataTable):

    def __init__(self, stats):
        super().__init__()
        self.stats = stats

    def on_mount(self):

        self.cursor_type = "none"

        self.add_columns(
            "[cyan]Resource",
            "[green]Final",
            "[yellow]Maximum",
        )

        resources = [
            "food",
            "linemate",
            "deraumere",
            "sibur",
            "mendiane",
            "phiras",
            "thystame",
        ]

        final = self.stats["final_inventory"]
        maxi = self.stats["max_inventory"]

        for r in resources:
            self.add_row(
                r,
                str(final.get(r, 0)),
                str(maxi.get(r, 0)),
            )


class Commands(Static):

    def __init__(self, stats):
        super().__init__()
        self.stats = stats

    def on_mount(self):

        total = self.stats["total_commands"]
        maximum = max(self.stats["commands"].values())

        txt = "[bold cyan]Command Distribution[/]\n\n"

        for cmd, value in sorted(
            self.stats["commands"].items(),
            key=lambda x: x[1],
            reverse=True,
        ):

            percent = value / total * 100

            width = 42
            filled = int(value / maximum * width)

            bar = (
                "[green]"
                + "█" * filled
                + "[grey23]"
                + "░" * (width - filled)
            )

            txt += (
                f"[yellow]{cmd:<12}[/] "
                f"{bar}[/] "
                f"[cyan]{percent:5.1f}%[/] "
                f"[white]{value}[/]\n"
            )

        self.update(txt)



class Dashboard(App):

    CSS = """

Screen{
    background:#111827;
}

Header{
    background:#0f766e;
    color:white;
}

Footer{
    background:#0f766e;
    color:white;
}

Summary{

    width:30%;
    margin:1;
    padding:1;

    border:round #06b6d4;

    background:#1e293b;
}

Inventory{

    width:70%;

    margin:1;

    border:round #22c55e;

    background:#1e293b;
}

Commands{

    margin:1;

    padding:1;

    border:round #f59e0b;

    background:#1e293b;

    height:1fr;
}

"""

    def __init__(self, stats):
        super().__init__()
        self.stats = stats

    def compose(self) -> ComposeResult:

        yield Header(show_clock=True)

        with Horizontal():

            yield Summary(self.stats)

            yield Inventory(self.stats)

        yield Commands(self.stats)

        yield Footer()


def viewer(the_file : str):


    with open(file=the_file) as f:
        stats = json.load(f)

    Dashboard(stats).run()


def main():

    if len(sys.argv) != 2:
        print("usage: python dashboard.py stats.json")
        exit(84)

    with open(sys.argv[1]) as f:
        stats = json.load(f)

    Dashboard(stats).run()


if __name__ == "__main__":
    main()