#include <stdio.h>
#include <stdlib.h>
#include <time.h>

/*
 * Connect 4 terminal implementation.
 *
 * Board model used in this file:
 * - grid[row][col]
 * - row 0 is the bottom row (where tokens land first)
 * - row ROW-1 is the top row
 *
 * This representation keeps drop logic simple: to insert a token in a column,
 * we scan rows from 0 upward and place it in the first free cell.
 */

#define ROW 6
#define COLUMN 7

#define ANSI_COLOR_RED "\x1b[31m"
#define ANSI_COLOR_GREEN "\x1b[32m"
#define ANSI_COLOR_YELLOW "\x1b[33m"
#define ANSI_COLOR_RESET "\x1b[0m"
#define ANSI_COLOR_GRAY "\x1b[0;37m"

int grid[ROW][COLUMN];

int chooseMode(void);
int getUserInt(int player);
int play(int currentPlayer, int isMultiplayer, int computer);
int checkWinner(void);
int checkWinnerBoard(int board[ROW][COLUMN]);
int gridInitializer(void);
int gridShow(void);
int myRand(int low, int high);
int isBoardFull(void);
int isValidColumn(int board[ROW][COLUMN], int col);
int dropToken(int board[ROW][COLUMN], int col, int player);
int chooseComputerMove(int computerPlayer);

int main(void)
{
    /* Seed randomness once so first player and AI tie-breaks vary per run. */
    srand((unsigned int)time(NULL));

    int player;
    int computer = 0;
    int isMultiplayer;

    /* 0 = vs computer, 1 = local multiplayer */
    isMultiplayer = chooseMode();

    /* Randomly choose who starts: player 1 or player 2. */
    player = myRand(1, 2);

    if (isMultiplayer == 0)
    {
        /* In single-player mode, computer uses the opposite id. */
        computer = (player == 1) ? 2 : 1;
        printf("\n%sComputer: Ok! I'll take number %d%s\n", ANSI_COLOR_GREEN, computer, ANSI_COLOR_RESET);
    }

    /* Start with an empty board. */
    gridInitializer();

    printf("\n<---------START--------->\n");
    printf(" %sFirst player is : %d%s", ANSI_COLOR_GREEN, player, ANSI_COLOR_RESET);
    printf("\n<----------------------->\n");

    gridShow();

    while (1)
    {
        /* Execute one move by current player (human or computer). */
        play(player, isMultiplayer, computer);

        /* Check winner immediately after each move. */
        if (checkWinner() != 0)
        {
            break;
        }

        /* If board is full and nobody has won, game is a draw. */
        if (isBoardFull())
        {
            break;
        }

        /* Alternate turns: 1 -> 2, 2 -> 1. */
        player = (player == 1) ? 2 : 1;
    }

    printf("\n<---------RESULT-------->\n");
    if (checkWinner() == 1)
    {
        printf(" %sPLAYER 1 wins%s", ANSI_COLOR_YELLOW, ANSI_COLOR_RESET);
    }
    else if (checkWinner() == 2)
    {
        printf(" %sPLAYER 2 wins%s", ANSI_COLOR_RED, ANSI_COLOR_RESET);
    }
    else
    {
        printf(" %sDRAW%s", ANSI_COLOR_GREEN, ANSI_COLOR_RESET);
    }
    printf("\n<----------------------->\n\n");

    return 0;
}

int chooseMode(void)
{
    int isMultiplayer = -1;

    /*
     * Keep asking until user inserts a valid mode.
     * scanf return value is checked to safely handle non-numeric input.
     */
    while (isMultiplayer != 0 && isMultiplayer != 1)
    {
        printf("%sWelcome to Connect4! press 0 to play with me or 1 to play with a friend: %s", ANSI_COLOR_GREEN, ANSI_COLOR_RESET);
        if (scanf("%d", &isMultiplayer) != 1)
        {
            /* Flush invalid token from stdin until newline. */
            while (getchar() != '\n')
            {
            }
            isMultiplayer = -1;
        }
    }

    return isMultiplayer;
}

int getUserInt(int player)
{
    int column;
    /* A move is represented by a column index in [0, COLUMN-1]. */
    printf("\n%sPlayer %d insert column: %s", ANSI_COLOR_GREEN, player, ANSI_COLOR_RESET);
    scanf("%d", &column);
    return column;
}

int play(int currentPlayer, int isMultiplayer, int computer)
{
    int col;

    if (isMultiplayer == 0 && currentPlayer == computer)
    {
        /* CPU turn: choose the best available column according to heuristic. */
        col = chooseComputerMove(computer);
        printf("\n%sComputer: I choose %d%s\n", ANSI_COLOR_GREEN, col, ANSI_COLOR_RESET);
    }
    else
    {
        /* Human turn: ask until value is inside range and column is playable. */
        col = getUserInt(currentPlayer);
        while (col < 0 || col >= COLUMN || !isValidColumn(grid, col))
        {
            printf("%sInvalid column, please insert a number from 0 to %d and choose a non-full column.%s\n", ANSI_COLOR_RED, COLUMN - 1, ANSI_COLOR_RESET);
            col = getUserInt(currentPlayer);
        }
    }

    /* Apply selected move to the live grid and print updated state. */
    dropToken(grid, col, currentPlayer);
    gridShow();
    return 0;
}

int checkWinner(void)
{
    /* Wrapper that evaluates the global grid. */
    return checkWinnerBoard(grid);
}

int checkWinnerBoard(int board[ROW][COLUMN])
{
    int i;
    int j;

    for (i = 0; i < ROW; i++)
    {
        for (j = 0; j < COLUMN; j++)
        {
            int token = board[i][j];
            if (token == 0)
            {
                /* Empty cells cannot start a winning sequence. */
                continue;
            }

            /* Horizontal check: (i,j) ... (i,j+3) */
            if (j + 3 < COLUMN &&
                board[i][j + 1] == token &&
                board[i][j + 2] == token &&
                board[i][j + 3] == token)
            {
                return token;
            }

            /* Vertical check: (i,j) ... (i+3,j) */
            if (i + 3 < ROW &&
                board[i + 1][j] == token &&
                board[i + 2][j] == token &&
                board[i + 3][j] == token)
            {
                return token;
            }

            /* Diagonal up-right check: (i,j) ... (i+3,j+3) */
            if (i + 3 < ROW && j + 3 < COLUMN &&
                board[i + 1][j + 1] == token &&
                board[i + 2][j + 2] == token &&
                board[i + 3][j + 3] == token)
            {
                return token;
            }

            /* Diagonal down-right check: (i,j) ... (i-3,j+3) */
            if (i - 3 >= 0 && j + 3 < COLUMN &&
                board[i - 1][j + 1] == token &&
                board[i - 2][j + 2] == token &&
                board[i - 3][j + 3] == token)
            {
                return token;
            }
        }
    }

    return 0;
}

int gridInitializer(void)
{
    int i;
    int j;

    /* Set every board cell to 0 (empty). */
    for (i = 0; i < ROW; i++)
    {
        for (j = 0; j < COLUMN; j++)
        {
            grid[i][j] = 0;
        }
    }

    return 0;
}

int gridShow(void)
{
    int tmpGrid[ROW][COLUMN];
    int i;
    int j;

    /*
     * Reverse rows only for rendering:
     * internal row 0 is bottom, but on screen we print top row first.
     */
    for (i = 0; i < ROW; i++)
    {
        for (j = 0; j < COLUMN; j++)
        {
            tmpGrid[ROW - i - 1][j] = grid[i][j];
        }
    }

    printf("\n");

    for (i = 0; i < ROW; i++)
    {
        for (j = 0; j < COLUMN; j++)
        {
            /* Player 1 = yellow, Player 2 = red, empty = gray. */
            if (tmpGrid[i][j] == 1)
            {
                printf("%s%d%s ", ANSI_COLOR_YELLOW, tmpGrid[i][j], ANSI_COLOR_RESET);
            }
            else if (tmpGrid[i][j] == 2)
            {
                printf("%s%d%s ", ANSI_COLOR_RED, tmpGrid[i][j], ANSI_COLOR_RESET);
            }
            else
            {
                printf("%s%d%s ", ANSI_COLOR_GRAY, tmpGrid[i][j], ANSI_COLOR_RESET);
            }
        }
        printf("\n");
    }

    for (i = 0; i < COLUMN; i++)
    {
        printf("--");
    }
    printf("-\n");
    for (i = 0; i < COLUMN; i++)
    {
        printf("%d ", i);
    }
    printf("\n");

    return 0;
}

int myRand(int low, int high)
{
    /* Inclusive random integer in [low, high]. */
    return rand() % (high - low + 1) + low;
}

int isBoardFull(void)
{
    int col;

    /* If any top cell is still empty, at least one move is available. */
    for (col = 0; col < COLUMN; col++)
    {
        if (grid[ROW - 1][col] == 0)
        {
            return 0;
        }
    }
    return 1;
}

int isValidColumn(int board[ROW][COLUMN], int col)
{
    /* Valid column index and not full. */
    return col >= 0 && col < COLUMN && board[ROW - 1][col] == 0;
}

int dropToken(int board[ROW][COLUMN], int col, int player)
{
    int row;

    /*
     * Gravity simulation:
     * place token in the first free row from bottom to top.
     */
    for (row = 0; row < ROW; row++)
    {
        if (board[row][col] == 0)
        {
            board[row][col] = player;
            return row;
        }
    }
    return -1;
}

int chooseComputerMove(int computerPlayer)
{
    /*
     * CPU strategy priority:
     * 1) Play immediate winning move.
     * 2) Block opponent immediate winning move.
     * 3) Prefer central columns (better tactical potential).
     * 4) Fallback random valid column.
     */
    int opponent = (computerPlayer == 1) ? 2 : 1;
    int boardCopy[ROW][COLUMN];
    int i;
    int j;
    int col;
    int preferredOrder[COLUMN] = {3, 2, 4, 1, 5, 0, 6};

    for (col = 0; col < COLUMN; col++)
    {
        if (!isValidColumn(grid, col))
        {
            continue;
        }

        /* Clone board to test hypothetical moves safely. */
        for (i = 0; i < ROW; i++)
        {
            for (j = 0; j < COLUMN; j++)
            {
                boardCopy[i][j] = grid[i][j];
            }
        }

        /* If computer can win now, take this move immediately. */
        dropToken(boardCopy, col, computerPlayer);
        if (checkWinnerBoard(boardCopy) == computerPlayer)
        {
            return col;
        }
    }

    for (col = 0; col < COLUMN; col++)
    {
        if (!isValidColumn(grid, col))
        {
            continue;
        }

        for (i = 0; i < ROW; i++)
        {
            for (j = 0; j < COLUMN; j++)
            {
                boardCopy[i][j] = grid[i][j];
            }
        }

        /* If opponent would win on this column, block it now. */
        dropToken(boardCopy, col, opponent);
        if (checkWinnerBoard(boardCopy) == opponent)
        {
            return col;
        }
    }

    for (i = 0; i < COLUMN; i++)
    {
        col = preferredOrder[i];
        /* Center-first ordering: 3,2,4,1,5,0,6. */
        if (isValidColumn(grid, col))
        {
            return col;
        }
    }

    /* Rare fallback (should happen only on edge states). */
    return myRand(0, COLUMN - 1);
}