#include <stdio.h>

typedef struct {
    int row;
    int column;
} Square;

typedef struct {
    Square squares[8];
    int count;
} SquareList;

int rowMoves[8] = {2, 1, -1, -2, -2, -1, 1, 2};
int columnMoves[8] = {1, 2, 2, 1, -1, -2, -2, -1};

SquareList availableSquares(int boardSize, Square currentSquare)
{
    SquareList list;
    int moveIndex, newRow, newColumn;

    list.count = 0;
    for (moveIndex = 0; moveIndex < 8; moveIndex++) {
        newRow = currentSquare.row + rowMoves[moveIndex];
        newColumn = currentSquare.column + columnMoves[moveIndex];
        if (newRow >= 0 && newRow < boardSize && newColumn >= 0 && newColumn < boardSize) {
            list.squares[list.count].row = newRow;
            list.squares[list.count].column = newColumn;
            list.count++;
        }
    }
    return list;
}

int isVisited(Square visitedStack[], int visitedTop, Square square)
{
    int index;

    for (index = 0; index <= visitedTop; index++) {
        if (visitedStack[index].row == square.row && visitedStack[index].column == square.column)
            return 1;
    }
    return 0;
}

int countNextMoves(int boardSize, Square square, Square visitedStack[], int visitedTop)
{
    SquareList list;
    int index, counter;

    list = availableSquares(boardSize, square);
    counter = 0;
    for (index = 0; index < list.count; index++) {
        if (!isVisited(visitedStack, visitedTop, list.squares[index]))
            counter++;
    }
    return counter;
}

SquareList unvisitedSquares(int boardSize, Square square, Square visitedStack[], int visitedTop)
{
    SquareList allSquares, result;
    int index, sortedIndex, previousIndex;
    int nextMovesCount[8];
    Square temporarySquare;
    int temporaryCount;

    allSquares = availableSquares(boardSize, square);
    result.count = 0;
    for (index = 0; index < allSquares.count; index++) {
        if (!isVisited(visitedStack, visitedTop, allSquares.squares[index])) {
            result.squares[result.count] = allSquares.squares[index];
            nextMovesCount[result.count] = countNextMoves(boardSize, allSquares.squares[index], visitedStack, visitedTop);
            result.count++;
        }
    }
    for (sortedIndex = 1; sortedIndex < result.count; sortedIndex++) {
        temporarySquare = result.squares[sortedIndex];
        temporaryCount = nextMovesCount[sortedIndex];
        previousIndex = sortedIndex - 1;
        while (previousIndex >= 0 && nextMovesCount[previousIndex] > temporaryCount) {
            result.squares[previousIndex + 1] = result.squares[previousIndex];
            nextMovesCount[previousIndex + 1] = nextMovesCount[previousIndex];
            previousIndex--;
        }
        result.squares[previousIndex + 1] = temporarySquare;
        nextMovesCount[previousIndex + 1] = temporaryCount;
    }
    return result;
}

void removeFirstSquare(SquareList *list)
{
    int index;

    for (index = 0; index < list->count - 1; index++)
        list->squares[index] = list->squares[index + 1];
    list->count--;
}

int main()
{
    Square visitedStack[64];
    SquareList availableStack[64];
    int moveNumbers[64];
    int visitedTop, availableTop;
    int boardSize, rowIndex, columnIndex;
    Square startSquare, nextSquare;

    printf("Enter the board size n (max 8): ");
    scanf("%d", &boardSize);
    printf("Enter the starting row and column (1 to %d): ", boardSize);
    scanf("%d %d", &startSquare.row, &startSquare.column);
    startSquare.row--;
    startSquare.column--;

    if (boardSize < 1 || boardSize > 8 || startSquare.row < 0 || startSquare.row >= boardSize ||
        startSquare.column < 0 || startSquare.column >= boardSize) {
        printf("Invalid input\n");
        return 1;
    }

    printf("Searching for a solution...\n");

    visitedTop = 0;
    availableTop = 0;
    visitedStack[0] = startSquare;
    availableStack[0] = unvisitedSquares(boardSize, startSquare, visitedStack, visitedTop);

    while (visitedTop >= 0 && visitedTop < boardSize * boardSize - 1) {
        if (availableStack[availableTop].count > 0) {
            nextSquare = availableStack[availableTop].squares[0];
            visitedTop++;
            visitedStack[visitedTop] = nextSquare;
            availableTop++;
            availableStack[availableTop] = unvisitedSquares(boardSize, nextSquare, visitedStack, visitedTop);
        } else {
            visitedTop--;
            availableTop--;
            if (availableTop >= 0)
                removeFirstSquare(&availableStack[availableTop]);
        }
    }

    if (visitedTop < 0) {
        printf("No solution found\n");
    } else {
        printf("Success: the knight visited all %d squares\n", boardSize * boardSize);
        for (rowIndex = 0; rowIndex < boardSize * boardSize; rowIndex++)
            moveNumbers[rowIndex] = 0;
        for (rowIndex = 0; rowIndex <= visitedTop; rowIndex++)
            moveNumbers[visitedStack[rowIndex].row * boardSize + visitedStack[rowIndex].column] = rowIndex + 1;
        for (rowIndex = 0; rowIndex < boardSize; rowIndex++) {
            for (columnIndex = 0; columnIndex < boardSize; columnIndex++)
                printf("%4d", moveNumbers[rowIndex * boardSize + columnIndex]);
            printf("\n");
        }
    }

    return 0;
}
