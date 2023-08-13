#!/bin/sh

echo "Built binary not found, compiling..."
cc src/main.c -o build/pbfi -lcurses
echo Done.
mkdir -p ~/.local/bin
cp build/pbfi ~/.local/bin/pbfi
echo Built binary placed in ~/.local/bin/. You may need to add ~/.local/bin/ to your \$PATH
