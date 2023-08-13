# bfi
A BrainF**k interpreter with added fuctionality, written in C!
Please don't expect this to perform the best, I'm not the best at optimizing my code! :p

## Compilation and Installation
To compile:
- `./compile.sh`

## Aditional Optional Features
- Allow the use of the `getch()` function instead of `scanf()`, so that you don't need to hit the enter key to input a character
- Adds `*`, which waits one millisecond for the number at the current cell, so if the value at the current cell is 125, it will wait 125 milliseconds (⅛ second).

## Planned features
- Load configs from `bf.conf` file (in progress).
- Run and receive shell commands in BrainF**k.
- Interact with files.
