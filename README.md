# Connect 4 🎯

Classic Connect 4 implemented in C, playable from terminal. 🧠

## Start the game 🚀

Compile the game:

```bash
make
```

Run it:

```bash
./Connect4
```

## How to play 🎮

Choose between local multiplayer and vs computer mode, then take turns dropping a token into one of the 7 columns.
Tokens fall to the lowest free row in the selected column.
The first player to connect 4 tokens (horizontal, vertical or diagonal) wins. 🏆

## Computer AI 🤖

The computer uses a practical strategy (not just random):

- Play an immediate winning move when available.
- Block your immediate winning move.
- Prefer central columns for stronger board control.

This keeps the game simple but noticeably smarter in single-player mode. 📈
