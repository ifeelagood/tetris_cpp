#include "piece.h"

#include "tetris.h"
#include <algorithm>


Matrix getMatrixFromType(Tetromino minoType)
{
    return MinoMatrix[minoType];
}

Piece::Piece(Tetromino minoType)
{
    this->minoType = minoType;
    this->matrix = getMatrixFromType(minoType);
    this->x = MinoPositions[minoType].first;
    this->y = MinoPositions[minoType].second;

    this->x += (PILE_WIDTH / 2) - 1;
    // this->y -= 1;
}

// helper functions for rotation
void transpose(Matrix &matrix)
{
    for (unsigned int y = 0; y < matrix.size(); y++)
    {
        for (unsigned int x = y; x < matrix[y].size(); x++)
        {
            std::swap(matrix[y][x], matrix[x][y]);
        }
    }
}

void reverseColumns(Matrix &matrix)
{
    for (unsigned int y = 0; y < (unsigned int) matrix.size(); y++)
    {
        std::reverse(matrix[y].begin(), matrix[y].end());
    }
}

void reverseRows(Matrix &matrix)
{
    std::reverse(matrix.begin(), matrix.end());
}


// Left rotation:   transpose -> row reverse
// Right rotation:  transpose -> col reverse
void Piece::rotateLeft()
{
    transpose(this->matrix);
    reverseRows(this->matrix);
}

void Piece::rotateRight()
{
    transpose(this->matrix);
    reverseColumns(this->matrix);
}