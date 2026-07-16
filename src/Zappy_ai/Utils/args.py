import argparse
import sys

def parse_args():
    parser = argparse.ArgumentParser(
        prog="zappy_ai",
        description="Zappy AI client",
        add_help=False
    )
    parser.add_argument("-p", type=int, required=True,  metavar="port")
    parser.add_argument("-n", type=str, required=True,  metavar="name")
    parser.add_argument("-h", type=str, required=False, metavar="machine",
                        default="localhost")
    parser.add_argument("--help", action="help",
                        help="show this help message and exit")

    args = parser.parse_args()

    if not (1 <= args.p <= 65535):
        print("Error: port must be between 1 and 65535", file=sys.stderr)
        sys.exit(84)

    return args