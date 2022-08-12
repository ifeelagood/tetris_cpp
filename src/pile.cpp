#include "pile.h"

#include <iostream>

Pile::Pile(const int w, const int h) : w(w), h(h)
{
    this->pile.resize(h);
    for (int i = 0; i < h; i++)
    {
        this->pile[i].resize(w);
    }
}


bool Pile::isRowFull(const int y)
{
    for (int x = 0; x < this->pile[y].size(); x++)
    {
        if (this->pile[y][x] == 0) { return false; }
    }

    return true;
}

void Pile::addPiece(const Piece &p)
{
    int px = p.getX();
    int py = p.getY();

    std::vector<std::vector<int>> shape = p.getShape();

    for (int y = 0; y < (int) shape.size(); y++)
    {
        for (int x = 0; x < (int) shape[y].size(); x++)
        {
            // TODO check for out of bounds
            if (shape[y][x] > 0)
            {
                this->pile[y + py][x + px] = shape[y][x];
            }
        }
    }
}

bool Pile::isTileEmpty(const int x, const int y)
{
    if ( (x < 0 || x > this->w - 1) || (y > this->h - 1) ) { return false; } // is out of bounds then treat as full
    else { return (this->pile[y][x] == 0); } // else return cell being empty
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

    std::vector<int> newRow(this->w);
    std::fill(newRow.begin(), newRow.end(), 0);
    this->pile.insert(this->pile.begin(), newRow);
}