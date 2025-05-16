#!/bin/zsh  
set -e

[ -d build ] || meson setup build
meson compile -C build
./build/src/main

