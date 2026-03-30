# Connect 4

Connect 4 in C.

# Start the game

To compile game write ```make``` in console, then start the game typing ```./Connect4```

## How to play

Using your terminal, choose to play with a friend or with computer then take turns dropping one colored disc from the top into a seven-column, six-row vertically suspended grid. The pieces fall straight down, occupying the lowest available space within the column. The objective of the game is to be the first to form a horizontal, vertical, or diagonal line of four of one's own discs.

## Computer AI

The computer opponent now uses a simple decision strategy instead of pure randomness:

- It plays a winning move if available.
- It blocks your immediate winning move.
- It prefers central columns (better long-term control of the board).

This keeps matches fast while making the single-player mode noticeably smarter.
