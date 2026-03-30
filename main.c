#include <stdio.h>
#include <stdlib.h>
#include <time.h>

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
    srand((unsigned int)time(NULL));

    int player;
    int computer = 0;
    int isMultiplayer;

    isMultiplayer = chooseMode();
    player = myRand(1, 2);

    if (isMultiplayer == 0)
    {
        computer = (player == 1) ? 2 : 1;
        printf("\n%sComputer: Ok! I'll take number %d%s\n", ANSI_COLOR_GREEN, computer, ANSI_COLOR_RESET);
    }

    gridInitializer();

    printf("\n<---------START--------->\n");
    printf(" %sFirst player is : %d%s", ANSI_COLOR_GREEN, player, ANSI_COLOR_RESET);
    printf("\n<----------------------->\n");

    gridShow();

    while (1)
    {
        play(player, isMultiplayer, computer);

        if (checkWinner() != 0)
        {
            break;
        }

        if (isBoardFull())
        {
            break;
        }

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

    while (isMultiplayer != 0 && isMultiplayer != 1)
    {
        printf("%sWelcome to Connect4! press 0 to play with me or 1 to play with a friend: %s", ANSI_COLOR_GREEN, ANSI_COLOR_RESET);
        if (scanf("%d", &isMultiplayer) != 1)
        {
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
    printf("\n%sPlayer %d insert column: %s", ANSI_COLOR_GREEN, player, ANSI_COLOR_RESET);
    scanf("%d", &column);
    return column;
}

int play(int currentPlayer, int isMultiplayer, int computer)
{
    int col;

    if (isMultiplayer == 0 && currentPlayer == computer)
    {
        col = chooseComputerMove(computer);
        printf("\n%sComputer: I choose %d%s\n", ANSI_COLOR_GREEN, col, ANSI_COLOR_RESET);
    }
    else
    {
        col = getUserInt(currentPlayer);
        while (col < 0 || col >= COLUMN || !isValidColumn(grid, col))
        {
            printf("%sInvalid column, please insert a number from 0 to %d and choose a non-full column.%s\n", ANSI_COLOR_RED, COLUMN - 1, ANSI_COLOR_RESET);
            col = getUserInt(currentPlayer);
        }
    }

    dropToken(grid, col, currentPlayer);
    gridShow();
    return 0;
}

int checkWinner(void)
{
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
                continue;
            }

            if (j + 3 < COLUMN &&
                board[i][j + 1] == token &&
                board[i][j + 2] == token &&
                board[i][j + 3] == token)
            {
                return token;
            }

            if (i + 3 < ROW &&
                board[i + 1][j] == token &&
                board[i + 2][j] == token &&
                board[i + 3][j] == token)
            {
                return token;
            }

            if (i + 3 < ROW && j + 3 < COLUMN &&
                board[i + 1][j + 1] == token &&
                board[i + 2][j + 2] == token &&
                board[i + 3][j + 3] == token)
            {
                return token;
            }

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
    return rand() % (high - low + 1) + low;
}

int isBoardFull(void)
{
    int col;
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
    return col >= 0 && col < COLUMN && board[ROW - 1][col] == 0;
}

int dropToken(int board[ROW][COLUMN], int col, int player)
{
    int row;
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

        for (i = 0; i < ROW; i++)
        {
            for (j = 0; j < COLUMN; j++)
            {
                boardCopy[i][j] = grid[i][j];
            }
        }

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

        dropToken(boardCopy, col, opponent);
        if (checkWinnerBoard(boardCopy) == opponent)
        {
            return col;
        }
    }

    for (i = 0; i < COLUMN; i++)
    {
        col = preferredOrder[i];
        if (isValidColumn(grid, col))
        {
            return col;
        }
    }

    return myRand(0, COLUMN - 1);
}