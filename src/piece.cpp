#include "piece.h"
#include <algorithm>

int Piece::IDs = 0;

Piece& Piece::operator=(Piece const &other)
{
    if (this != &other)
    {
        this->id    = other.id;
        this->x     = other.x;
        this->y     = other.y;
        this->shape = other.shape;
        this->pieceShape = other.pieceShape;
    }

    return *this;
}


void Piece::translate(int dx, int dy)
{
    this->x += dx;
    this->y += dy;
}

// thank you to https://stackoverflow.com/a/8664879/19093477 for this algorithm!
void Piece::rotateLeft()
{
    std::vector<std::vector<int>> rotatedShape{shape};

    // 1. Transpose
    for (int y = 0; y < this->shape.size(); y++)
    {
        for (int x = 0; x < this->shape[y].size(); x++)
        {
            rotatedShape[x][y] = this->shape[y][x];
        }
    }

    // 2. Reverse rows
    std::reverse(rotatedShape.begin(), rotatedShape.end());


    this->setShape(rotatedShape);
}

void Piece::rotateRight()
{
    std::vector<std::vector<int>> rotatedShape{shape};

    // 1. Transpose
    for (int y = 0; y < this->shape.size(); y++)
    {
        for (int x = 0; x < this->shape[y].size(); x++)
        {
            rotatedShape[x][y] = this->shape[y][x];
        }
    }

    // 2. Reverse Columns
    for (int y = 0; y < this->shape.size(); y++)
    {
        std::reverse(rotatedShape[y].begin(), rotatedShape[y].end());
    }

    this->setShape(rotatedShape);
}