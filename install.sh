#!/bin/sh

if test -f "build/pbfi"; then
	cp build/pbfi ~/.local/bin/pbfi
else
	echo "Built binary not found, compiling..."
	cc src/main.c -o build/pbfi -lcurses
	cp build/pbfi ~/.local/bin/pbfi
fi
