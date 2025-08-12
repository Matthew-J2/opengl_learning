#!/bin/zsh  
set -e

ROOT_DIR=$(pwd)

if [[ $1 == rebuild ]]; then
  rm -rf build
fi

[ -d build ] || meson setup build
meson compile -C build

cd "$ROOT_DIR/build"
./src/main

