#include "board.h"
#include "common.h"
#include "piece.h"
#include "tetris.h"

#include <vector>
#include <algorithm>



void BoardManager::reset()
{
    this->pile.clear();
    this->activePiece = Piece();
}

bool BoardManager::isColliding(Piece const& p)
{
    // check if piece is colliding with pile OR
    // if piece is colliding with left/right wall or the bottom

    Matrix pieceShape = p.getMatrix();
    Matrix pileShape = this->pile.getMatrix();

    for (unsigned int y = 0; y < (unsigned int) pieceShape.size(); y++)
    {
        for (unsigned int x = 0; x < (unsigned int) pieceShape[y].size(); x++)
        {
            if (pieceShape[y][x] > 0)
            {
                // IF OOB
                if ((p.y + y) >= PILE_HEIGHT || (p.x + x) < 0 || (p.x + x) >= PILE_WIDTH)
                {
                    return true;
                }
                // IF COLLIDING WITH PILE
                if ((p.y + y) >= 0 && pileShape[p.y + y][p.x + x] > 0)
                {
                    return true;
                }
            }
        }
    }

    return false;
}


// movement / rotation functions are just proxies, which only allow movement when its possible
void BoardManager::moveActiveLeft()
{
    Piece deltaPiece = this->activePiece;
    deltaPiece.x -= 1;

    if (!this->isColliding(deltaPiece))
    {
        this->activePiece = deltaPiece;
    }
}

void BoardManager::moveActiveRight()
{
    Piece deltaPiece = this->activePiece;
    deltaPiece.x += 1;

    if (!this->isColliding(deltaPiece))
    {
        this->activePiece = deltaPiece;
    }
}

void BoardManager::moveActiveDown()
{
    Piece deltaPiece = this->activePiece;
    deltaPiece.y += 1;

    if (!this->isColliding(deltaPiece))
    {
        this->activePiece = deltaPiece;
    }
}

void BoardManager::rotateActiveLeft()
{
    Piece deltaPiece = this->activePiece;
    deltaPiece.rotateLeft();
    if (!this->isColliding(deltaPiece))
    {
        this->activePiece = deltaPiece;
    }

}


void BoardManager::rotateActiveRight()
{
    Piece deltaPiece = this->activePiece;
    deltaPiece.rotateRight();
    if (!this->isColliding(deltaPiece))
    {
        this->activePiece = deltaPiece;
    }
}

// isLanded checks if active piece when moved down is colliding with pile or bottom

bool BoardManager::isLanded()
{
    Piece deltaPiece = this->activePiece;
    deltaPiece.y += 1;

    return (this->isColliding(deltaPiece) || deltaPiece.y >= PILE_HEIGHT);
}

bool BoardManager::lockPiece()
{
    // check for top out
    if (this->isColliding(this->activePiece))
    {
        return true;
    }
    else
    {
        this->pile.addPiece(this->activePiece);
        return false;
    }
}

unsigned char BoardManager::clearLines()
{
    unsigned char linesCleared = this->pile.clearRows();
    return linesCleared;
}

void BoardManager::setActivePiece(Piece p)
{
    this->activePiece = p;
}