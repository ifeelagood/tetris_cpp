#pragma once

#include "common.h"
#include "piece.h"

class Pile
{
private:
    Matrix pile;
    void resize();

    bool isRowFull(int y);
    void clearRow(int y);

public:
    // CONSTRUCTORS
    Pile();
    ~Pile() {};

    Matrix getMatrix() const { return this->pile; };
    bool isCellOccupied(int x, int y) const { return this->pile[y][x] > 0; };

    unsigned char clearRows(); // clears rows and returns number of rows cleared
    void addPiece(const Piece &p); // adds a Piece object to the pile
    void clear(); // clears the pile. duh
};