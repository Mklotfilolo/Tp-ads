#include <stdio.h>

int rowMoves[8] = {2, 1, -1, -2, -2, -1, 1, 2};
int columnMoves[8] = {1, 2, 2, 1, -1, -2, -2, -1};

int boardSize;
int visitedStack[64][2];
int visitedTop;
int availableStack[64][8][2];
int availableCount[64];
int availableTop;

int availableSquares(int row, int column, int list[8][2])
{
    int moveIndex, newRow, newColumn;
    int count = 0;

    for (moveIndex = 0; moveIndex < 8; moveIndex++) {
        newRow = row + rowMoves[moveIndex];
        newColumn = column + columnMoves[moveIndex];
        if (newRow >= 0 && newRow < boardSize && newColumn >= 0 && newColumn < boardSize) {
            list[count][0] = newRow;
            list[count][1] = newColumn;
            count++;
        }
    }
    return count;
}

int isVisited(int row, int column)
{
    int index;

    for (index = 0; index <= visitedTop; index++) {
        if (visitedStack[index][0] == row && visitedStack[index][1] == column)
            return 1;
    }
    return 0;
}

int unvisitedSquares(int row, int column, int list[8][2])
{
    int allSquares[8][2];
    int allCount, index;
    int count = 0;

    allCount = availableSquares(row, column, allSquares);
    for (index = 0; index < allCount; index++) {
        if (!isVisited(allSquares[index][0], allSquares[index][1])) {
            list[count][0] = allSquares[index][0];
            list[count][1] = allSquares[index][1];
            count++;
        }
    }
    return count;
}

void removeFirstSquare(int level)
{
    int index;

    for (index = 0; index < availableCount[level] - 1; index++) {
        availableStack[level][index][0] = availableStack[level][index + 1][0];
        availableStack[level][index][1] = availableStack[level][index + 1][1];
    }
    availableCount[level]--;
}

int main()
{
    int board[8][8];
    int startRow, startColumn;
    int nextRow, nextColumn;
    int rowIndex, columnIndex, index;

    printf("Enter the board size (8): ");
    scanf("%d", &boardSize);
    printf("Enter the starting row and column (1 to %d): ", boardSize);
    scanf("%d %d", &startRow, &startColumn);
    startRow--;
    startColumn--;

    if (boardSize < 1 || boardSize > 8 || startRow < 0 || startRow >= boardSize ||
        startColumn < 0 || startColumn >= boardSize) {
        printf("Invalid input\n");
        return 1;
    }
    
    printf("Searching for a solution...\n");

    visitedTop = 0;
    visitedStack[0][0] = startRow;
    visitedStack[0][1] = startColumn;
    availableTop = 0;
    availableCount[0] = unvisitedSquares(startRow, startColumn, availableStack[0]);

    while (visitedTop >= 0 && visitedTop < boardSize * boardSize - 1) {
        if (availableCount[availableTop] > 0) {
            nextRow = availableStack[availableTop][0][0];
            nextColumn = availableStack[availableTop][0][1];
            visitedTop++;
            visitedStack[visitedTop][0] = nextRow;
            visitedStack[visitedTop][1] = nextColumn;
            availableTop++;
            availableCount[availableTop] = unvisitedSquares(nextRow, nextColumn, availableStack[availableTop]);
        } else {
            visitedTop--;
            availableTop--;
            if (availableTop >= 0)
                removeFirstSquare(availableTop);
        }
    }

    if (visitedTop < 0) {
        printf("No solution found\n");
    } else {
        printf("Success\n");
        for (index = 0; index <= visitedTop; index++)
            board[visitedStack[index][0]][visitedStack[index][1]] = index + 1;
        for (rowIndex = 0; rowIndex < boardSize; rowIndex++) {
            for (columnIndex = 0; columnIndex < boardSize; columnIndex++)
                printf("%4d", board[rowIndex][columnIndex]);
            printf("\n");
        }
    }

    return 0;
}
