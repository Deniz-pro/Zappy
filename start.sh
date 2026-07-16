#!/bin/bash

set -e

if [ ! -d ".venv" ]; then
    echo "Virtual environment not found."
    echo "Run ./install.sh first."
    exit 1
fi

source .venv/bin/activate

.venv/bin/python zappy_ai "$@"