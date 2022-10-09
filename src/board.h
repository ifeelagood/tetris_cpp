#pragma once

#include "common.h"
#include "piece.h"
#include "pile.h"


class BoardManager
{
private:
    Pile pile;
    Piece activePiece;

    bool isColliding(Piece const& p);

public:
    BoardManager() {};
    ~BoardManager() {};

    // below are proxy functions for transforming the active piece.
    void moveActiveLeft();
    void moveActiveRight();
    void moveActiveDown();
    void rotateActiveLeft();
    void rotateActiveRight();

    bool isLanded(); // checks if piece will land.

    bool lockPiece(); // locks the active piece to the pile. returns true if topped out (if any block overlaps pile)
    unsigned char clearLines(); // clears lines and returns number of lines cleared
    void setActivePiece(Piece p);

    // getters
    Pile getPile() const { return this->pile; };
    Piece getActivePiece() const { return this->activePiece; };

    void reset(); 
};