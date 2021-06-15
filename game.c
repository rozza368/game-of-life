#include <stdio.h>
#define BOARD_W 80
#define BOARD_H 24

typedef struct {
    size_t x, y;
} vector2;

int areVectorsEqual(vector2 first, vector2 second)
{
    return first.x == second.x && first.y == second.y;
}

int vectorArea(vector2 v)
{
    return v.x * v.y;
}

int posToNum(vector2 pos, vector2 size)
{
    if (pos.x < size.x && pos.y < size.y && pos.x >= 0 && pos.y >= 0)
        return pos.y*size.x + pos.x;
    return -1;
}

vector2 numToPos(int num, vector2 size)
{
    vector2 result = {
        num % size.x,
        num / size.x
    };
    return result;
}

int neighbours(vector2 size, vector2 pos, int* arr)
{
    int count = 0;
    for (int x = pos.x - 1; x <= (pos.x + 1); x++)
    {
        for (int y = pos.y - 1; y <= (pos.y + 1); y++)
        {
            // printf("checking %d, %d ", x, y);
            vector2 checkPos = {x, y};
            /*
            printf("%d,%d\t%d\t%d\t%d",
                x, y,
                !areVectorsEqual(pos, checkPos),
                posToNum(checkPos, size),
                *(arr + posToNum(checkPos, size))
            );
            */
            if (!areVectorsEqual(pos, checkPos) && *(arr + posToNum(checkPos, size)) == 1)
            {
                count++;
            }
        }
    }
    return count;
}

void updateInto(int* originalBoard, int* nextBoard, vector2 boardSize)
{
    // need to iterate through first board; update
    // second board to match.
    for (int x = 0; x < boardSize.x; x++)
    {
    for (int y = 0; y < boardSize.y; y++)
    {
        vector2 currentPos = {x, y};
        int neighbourCount = neighbours(boardSize, currentPos, originalBoard);
        int* posPtr = nextBoard + posToNum(currentPos, boardSize);
        if (*(originalBoard + posToNum(currentPos, boardSize)) == 1) // starts live
        {
            if (neighbourCount < 2)
            {
                *(posPtr) = 0;
            }
            else if (neighbourCount == 2 || neighbourCount == 3)
            {
                // lives
                *(posPtr) = 1;
            }
            else if (neighbourCount > 3)
            {
                // dies by overpopulation
                *(posPtr) = 0;
            }
        }
        else // starts dead
        {
            if (neighbourCount == 3)
            {
                // become alive
                *(posPtr) = 1;
            }
        }
    }
    }
}

void printBoard(int* board, vector2 dims)
{
    int area = vectorArea(dims);
    for (int c = 0; c < area; c++)
    {
        if (c % dims.x == 0)
        {
            printf("\n");
        }

        if (*(board + c) == 0)
        {
            // print dead
            printf("-");
        }
        else
        {
            // print alive
            printf("+");
        }
    }
}

// TODO
void loadFileToBoard(FILE file, int* board)
{
    //
}

int main()
{
    vector2 boardSize = {BOARD_W, BOARD_H};
    int board[BOARD_H][BOARD_W] = {0};
    int boardBuf[BOARD_H][BOARD_W] = {0};
    int* boardPtr = &board[0][0];
    int* boardBufPtr = &boardBuf[0][0];
    // initial state
    //    y   x
    board[20][32] = 1;
    board[20][33] = 1;
    board[20][34] = 1;
    board[21][32] = 1;
    board[21][33] = 1;
    board[21][34] = 1;
    board[21][33] = 1;

    /*
    vector2 testPos = {1,1};
    printf("%d neighbours\n", neighbours(boardSize, testPos, boardPtr));
    */

    FILE* boardFile = fopen("board.txt", "r");

    while (1)
    {
        printBoard(boardPtr, boardSize);
        getchar();
        updateInto(boardPtr, boardBufPtr, boardSize);
        printBoard(boardBufPtr, boardSize);
        getchar();
        updateInto(boardBufPtr, boardPtr, boardSize);
    }

    return 0;
}
