#include "pile.h"
#include "common.h"
#include "tetris.h"

#include <iostream>

Pile::Pile()
{
    // set the pile size
    this->pile.resize(PILE_HEIGHT);
    for (int i = 0; i < PILE_HEIGHT; i++)
    {
        this->pile[i].resize(PILE_WIDTH);
    }
}

bool Pile::isRowFull(const int y)
{
    for (unsigned int x = 0; x < (unsigned int) this->pile[y].size(); x++)
    {
        if (this->pile[y][x] == 0) { return false; }
    }

    return true;
}

void Pile::addPiece(const Piece &p)
{
    Matrix pieceMatrix = p.getMatrix();

    for (unsigned int y = 0; y < (unsigned int) pieceMatrix.size(); y++)
    {
        for (unsigned int x = 0; x < (unsigned int) pieceMatrix[y].size(); x++)
        {
            if (pieceMatrix[y][x] > 0)
            {
                this->pile[y + p.y][x + p.x] = pieceMatrix[y][x]; // transform piece matrix to pile matrix
            }
        }
    }
}

void Pile::clear()
{
    std::fill(this->pile[0].begin(), this->pile[0].end(), 0);
    std::fill(this->pile.begin(), this->pile.end(), this->pile[0]);
}

void Pile::clearRow(const int y)
{
    this->pile.erase(this->pile.begin() + y);
    
    // add new empty row
    std::vector<unsigned char> newRow(PILE_WIDTH);
    std::fill(newRow.begin(), newRow.end(), 0);
    this->pile.insert(this->pile.begin(), newRow);
}

unsigned char Pile::clearRows()
{
    unsigned char rowsCleared = 0;

    for (int y = 0; y < PILE_HEIGHT; y++)
    {
        if (this->isRowFull(y))
        {
            this->clearRow(y);
            rowsCleared++;
        }
    }

    return rowsCleared;
}