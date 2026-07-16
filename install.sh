#!/bin/bash

set -e

echo " Installing Zappy AI environment "

if [ ! -d ".venv" ]; then
    echo "[+] Creating virtual environment..."
    python3 -m venv .venv
fi

source .venv/bin/activate

python -m pip install --upgrade pip
pip install -r requirements.txt

echo
echo " Installation complete!"
echo
echo "Run with:"
echo "./start.sh"
