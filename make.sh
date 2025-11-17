#!/bin/bash


if [ "$1" == "" -o "$1" == "help" -o "$1" == "--help" ]; then
  echo "USAGE: $(basename $0) [setup|clean|build]"
  echo "  setup: Install dependencies (will request sudo rights)."
  echo "  clean: Delete build files."
  echo "  build: Build the project with cmake."
fi

# Tested on Ubuntu 24.04:
if [ "$1" == "setup" ]; then
  sudo apt install build-essential git cmake libx11-dev libxrandr-dev libxinerama-dev libxcursor-dev libfreetype6-dev libwebkitgtk-6.0-dev libgtk-3-dev
  exit
fi

if [ "$1" == "clean" ]; then
  rm -rf build
  exit
fi

if [ "$1" == "build" ]; then
  mkdir -p build
  cd build
  cmake .. && cmake --build . -j
  exit
fi

echo "Option $1 is unknown. Use --help to display usage info."
