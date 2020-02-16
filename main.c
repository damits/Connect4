#include <stdio.h>
#include <time.h>
#include <stdlib.h>

#define ROW 6
#define COLUMN 7

#define ANSI_COLOR_RED "\x1b[31m"
#define ANSI_COLOR_GREEN "\x1b[32m"
#define ANSI_COLOR_YELLOW "\x1b[33m"
#define ANSI_COLOR_BLUE "\x1b[34m"
#define ANSI_COLOR_MAGENTA "\x1b[35m"
#define ANSI_COLOR_CYAN "\x1b[36m"
#define ANSI_COLOR_RESET "\x1b[0m"
#define ANSI_COLOR_GRAY "\e[0;37m"

int grid[ROW][COLUMN];

int main()
{
    struct timeval t1;
    gettimeofday(&t1, NULL);
    srand(t1.tv_usec * t1.tv_sec);

    int player;
    int computer;
    int isMultiplayer;

    // Choose a player
    isMultiplayer = chooseMode();
    if (isMultiplayer == 1)
    {
        player = myRand(1, 2);
    }
    else
    {
        player = myRand(1, 2);
        if (player == 1) {
            computer = 2;
        } else {
            computer = player;
        }
        printf("\n%sComputer: Ok! I'll take number %d%s\n", ANSI_COLOR_GREEN, computer, ANSI_COLOR_RESET);
    }

    // Create and Initialize the grid
    gridInitializer();

    printf("\n<---------START--------->\n");
    printf(" %sFirst player is : %d%s", ANSI_COLOR_GREEN, player, ANSI_COLOR_RESET);
    printf("\n<----------------------->\n\n");

    // Print the grid
    if (isMultiplayer == 1) {
        gridShow();
    }

    // Play game and find the winner
    int winner = 0;
    while (1)
    {
        if (player == 1)
        {
            // Player one start
            play(1, isMultiplayer, computer);

            // Check the winner
            if (checkWinner() == 1)
            {
                winner = checkWinner();
                break;
            }

            // Change the player
            player = 2;
        }
        else
        {
            // Player two start
            play(2, isMultiplayer, computer);

            // Check the winner
            if (checkWinner() == 2)
            {
                winner = checkWinner();
                break;
            }

            // Change the player
            player = 1;
        }
    }

    // Print the winner
    printf("\n<---------WINNER-------->\n");
    if (winner == 1)
    {
        printf(" %sPLAYER %d%s", ANSI_COLOR_YELLOW, winner, ANSI_COLOR_RESET);
    }
    else if (winner == 2)
    {
        printf(" %sPLAYER %d%s", ANSI_COLOR_RED, winner, ANSI_COLOR_RESET);
    }
    printf("\n<----------------------->\n\n");

    return 0;
}

int chooseMode()
{
    int isMultiplayer;
    printf("%sWelcome to Connect4! press 0 to play with me or 1 to play with a friend: %s", ANSI_COLOR_GREEN, ANSI_COLOR_RESET);
    scanf("%d", &isMultiplayer);
    return isMultiplayer;
}

int getUserInt(player)
{
    int column;
    printf("\n%sPlayer %d insert column: %s", ANSI_COLOR_GREEN, player, ANSI_COLOR_RESET);
    scanf("%d", &column);
    return column;
}

int play(int _player, int _isMultiplayer, int _computer)
{
    int col;
    if (_isMultiplayer == 0 && _player == _computer)
    {
        col = myRand(0, ROW);
        // Check row if empty
        while (grid[ROW][col] != 0)
        {
            col = myRand(0, ROW);
        }
         
        printf("\n%sComputer: I choose %d%s\n", ANSI_COLOR_GREEN, col, ANSI_COLOR_RESET);
    }
    else
    {
        col = getUserInt(_player);
        while(col < 0 || col > ROW) {
            printf("%sInvalid column, please insert a number from 0 to %d%s\n", ANSI_COLOR_RED, ROW, ANSI_COLOR_RESET);
            col = getUserInt(_player);
        }
    }

    // Check row if empty
    int i;
    for (i = 0; i < ROW; i++)
    {
        int lastRow = ROW - 1;
        if (grid[i][col] == 0)
        {
            grid[i][col] = _player;
            break;
        }
        else if (i == lastRow && grid[i][col] != 0) {
            printf("%sOoops, your token has fallen out! Pay attention!%s\n", ANSI_COLOR_RED, ANSI_COLOR_RESET);
            col = getUserInt(_player);
            i = -1;
        }
    }
    

    gridShow();

    return 0;
}

int checkWinner()
{
    int result = 0;
    int i, j;
    for (i = 0; i < ROW; i++)
    {
        for (j = 0; j < COLUMN; j++)
        {

            if (grid[i][j] != 0)
            {
                // Check line
                if (checkLine(i, j) != 0)
                {
                    result = checkLine(i, j);
                    break;
                    // Check Diagonal
                }
                else if (checkLeftDiagonal(i, j) != 0)
                {
                    result = checkLeftDiagonal(i, j);
                    break;
                }
                else if (checkRightDiagonal(i, j) != 0)
                {
                    result = checkRightDiagonal(i, j);
                    break;
                }
                else if (checkVertical(i,j) != 0) 
                {
                    result = checkVertical(i,j);
                    break;
                }
            }
        }
    }

    return result;
}

int checkVertical(int _i, int _j)
{

    if ((_i + 3) <= ROW)
    {
        int cell = grid[_i][_j];
        int cell_1 = grid[_i + 1][_j];
        int cell_2 = grid[_i + 2][_j];
        int cell_3 = grid[_i + 3][_j];

        if (cell_1 == cell && cell_2 == cell && cell_3 == cell)
        {
            // Return the winner number
            return cell;
        }
    }

    return 0;
}

int checkLine(int _i, int _j)
{

    if ((_j + 3) <= COLUMN)
    {
        int cell = grid[_i][_j];
        int cell_1 = grid[_i][_j + 1];
        int cell_2 = grid[_i][_j + 2];
        int cell_3 = grid[_i][_j + 3];

        if (cell_1 == cell && cell_2 == cell && cell_3 == cell)
        {
            // Return the winner number
            return cell;
        }
    }

    return 0;
}

int checkRightDiagonal(int _i, int _j)
{

    if (((_i + 3) <= ROW) && ((_j + 3) <= COLUMN))
    {
        int cell = grid[_i][_j];
        int cell_1 = grid[_i + 1][_j + 1];
        int cell_2 = grid[_i + 2][_j + 2];
        int cell_3 = grid[_i + 3][_j + 3];

        if (cell_1 == cell && cell_2 == cell && cell_3 == cell)
        {
            // Return the winner number
            return cell;
        }
    }

    return 0;
}

int checkLeftDiagonal(int _i, int _j)
{

    if (((_i + 3) <= ROW) && ((_j + 3) <= COLUMN))
    {
        int cell = grid[_i][_j];
        int cell_1 = grid[_i - 1][_j + 1];
        int cell_2 = grid[_i - 2][_j + 2];
        int cell_3 = grid[_i - 3][_j + 3];

        if (cell_1 == cell && cell_2 == cell && cell_3 == cell)
        {
            // Return the winner number
            return cell;
        }
    }

    return 0;
}

int gridInitializer()
{
    int i, j;
    for (i = 0; i < ROW; i++)
    {
        for (j = 0; j < COLUMN; j++)
        {
            grid[i][j] = 0;
        }
    }

    return 0;
}

int gridShow()
{
    int tpmGrid[ROW][COLUMN];
    int i, j;
    for (i = 0; i < ROW; i++)
    {
        for (j = 0; j < COLUMN; j++)
        {
            //printf("%d ", grid[i][j]);
            tpmGrid[ROW - i - 1][j] = grid[i][j];
            //printf("a[%d][%d] = %d\n", i,j, grid[i][j] );
        }
        // printf("\n");
    }

    printf("\n");

    for (i = 0; i < ROW; i++)
    {
        for (j = 0; j < COLUMN; j++)
        {
            if (tpmGrid[i][j] == 1)
            {
                printf("%s%d%s ", ANSI_COLOR_YELLOW, tpmGrid[i][j], ANSI_COLOR_RESET);
            }
            else if (tpmGrid[i][j] == 2)
            {
                printf("%s%d%s ", ANSI_COLOR_RED, tpmGrid[i][j], ANSI_COLOR_RESET);
            }
            else if (tpmGrid[i][j] == 0)
            {
                printf("%s%d%s ", ANSI_COLOR_GRAY, tpmGrid[i][j], ANSI_COLOR_RESET);
            }
        }
        printf("\n");
    }

    for (i = 0; i <= ROW; i++) {
        printf("--");
    }
    printf("\n");
    for (i = 0; i <= ROW; i++) {
        printf("%d ", i);
    }
    printf("\n");

    return 0;
}

int myRand(int low, int high)
{
    return rand() % (high - low + 1) + low;
}