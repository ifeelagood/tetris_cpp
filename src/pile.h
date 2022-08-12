#pragma once

#include <vector>
#include "piece.h"

typedef std::vector<std::vector<int>> Cells;


class Pile
{
private:
    int w,h;
    std::vector<std::vector<int>> pile;

public:
    Pile(const int w, const int h);

    // getters
    int getPileWidth()  const { return this->w; }
    int getPileHeight() const { return this->h; }
    std::vector<std::vector<int>> getPile() const { return pile; };

    bool isRowFull(const int y);

    void clearRow(const int y);
    void addPiece(const Piece &p);
    void clear();

    bool isTileEmpty(const int x, const int y);
};