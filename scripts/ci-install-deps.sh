#!/bin/bash
# Installe toutes les dépendances système pour la CI self-hosted Epitech.
set -euo pipefail

sudo apt-get update -qq
sudo apt-get install -y \
  build-essential \
  clang \
  wget \
  netcat-openbsd \
  xvfb \
  python3 \
  python3-pip \
  python3-venv \
  libcriterion-dev \
  libgl1-mesa-dev \
  libx11-dev \
  libxrandr-dev \
  libxcursor-dev \
  libxi-dev \
  libxinerama-dev

echo "Vérification des commandes requises"
missing=0
for cmd in make clang++ wget nc python3 tar timeout; do
  if command -v "$cmd" >/dev/null 2>&1; then
    echo "OK  $cmd -> $(command -v "$cmd")"
  else
    echo "MISSING  $cmd"
    missing=1
  fi
done

if [ "$missing" -ne 0 ]; then
  echo "Certaines commandes sont manquantes après installation."
  exit 1
fi
