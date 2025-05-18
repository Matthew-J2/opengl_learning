#!/bin/zsh  
set -e

if [[ $1 == rebuild ]]; then
  rm -rf build
fi

[ -d build ] || meson setup build
meson compile -C build
./build/src/main

