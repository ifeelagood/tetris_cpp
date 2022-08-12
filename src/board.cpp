#include "board.h"
#include "main.h"
#include "das.h"
#include "levels.h"
#include <iostream>


Board::Board(const int w, const int h)
{
    this->BoardWidth = w;
    this->BoardHeight = h + this->HiddenRows;
    this->pile.resize(this->BoardWidth, this->getBoardHeight());

    this->resetGame();
}

void Board::levelUp()
{
    this->level++;
    this->gravity = LevelGravity[this->level];

    std::cout << "LEVEL UP! Level: " << this->level << '\n';
}

void Board::update(Keys const &keys)
{
    // increment timer and counter
    this->timer.tick();
    this->counter.tick();

    // spawn new piece + check for top out
    if (!this->hasPiece)
    {
        this->spawnPiece();
        this->hasPiece = true;

        if (this->checkPileCollision(this->piece)) // TOPPED OUT
        {
            this->resetGame();
        }
    }

    // IO FUNCTIONS

    // soft drop and move piece down
    if (keys.down & (counter.get() % 2 == 0)) { this->translatePiece(0, 1); }
    if (counter.get() % this->gravity == 0) { this->lowerActivePiece(); }

    // rotate pieces
    if (this->oldKeys.z == 0 && keys.z == 1) { this->rotatePieceRight(); }
    if (this->oldKeys.x == 0 && keys.x == 1) { this->rotatePieceLeft(); }

    // DAS
    // if left or right are down, but not both (as left and right translation do nothing)
    this->das.update(this->oldKeys, keys);
    if (keys.left ^ keys.right)
    {
        // if das is ready for movement
        if (this->das.isReady())
        {
            if (keys.left ) { this->translatePiece(-1, 0);  }
            if (keys.right) { this->translatePiece(1, 0);   }
        }
    }

    // done processing input, so copy keys to this->oldKeys
    this->oldKeys = keys;


    // clear complete rows
    int cleared = 0; // 4 is tetris!
    for (int y = 0; y < this->pile.getPileHeight(); y++)
    {
        if (this->pile.isRowFull(y))
        {
            this->pile.clearRow(y);
            cleared++;
        }
    }
    if (cleared == 4) { std::cout << "TETRIS!!1!\n"; }
    this->linesCleared += cleared;

    // check for a level up
    if (this->linesCleared > (this->level + 1) * 10) { this->levelUp(); }
}

void Board::resetGame()
{
    this->pile.clear();
    this->pieceFactory.resetBag();

    this->score = 0;
    this->linesCleared = 0;
    this->level = 0;
    this->gravity = LevelGravity[level];

    this->spawnPiece();
}


bool Board::canTransform(Piece const &deltaPiece)
{
    if (this->outOfBounds(deltaPiece))
    {
        return false;
    }
    else if (this->checkPileCollision(deltaPiece))
    {
        return false;
    }
    else
    {
        return true;
    }

    // return (!this->outOfBounds(deltaPiece) & !this->checkPileCollision(deltaPiece)); // <- fucked
}

bool Board::checkPileCollision(Piece const &p)
{
	auto s = p.getShape();

    int px = p.getX();
    int py = p.getY();

    for (int dy = 0; dy < s.size(); dy++)
    {
        for (int dx = 0; dx < s[dy].size(); dx++)
        {
            int x = dx + px;
            int y = dy + py;

            if (s[dy][dx] == 1 && this->pile.getPile()[y][x] != 0)
            {
                return true;
            }
        }
    }
    return false;
}

bool Board::outOfBounds(Piece const &p)
{
	auto s = p.getShape();

    int px = p.getX();
    int py = p.getY();

    for (int dy = 0; dy < s.size(); dy++)
    {
        for (int dx = 0; dx < s[dy].size(); dx++)
        {
            int x = dx + px;
            int y = dy + py;

            if (s[dy][dx] == 1)
            {
                if (x < 0 || x >= this->BoardWidth || y >= this->BoardHeight) { return true; }
            }
        }
    }
    return false;
}

bool Board::translatePiece(const int dx, const int dy)
{
    Piece deltaPiece = this->piece;

    deltaPiece.translate(dx, dy);

    if (this->canTransform(deltaPiece)) { this->piece = deltaPiece; return true; }
    else {  return false; }
}

bool Board::rotatePieceRight()
{
    Piece deltaPiece = this->piece;

    deltaPiece.rotateRight();

    if (this->canTransform(deltaPiece)) { this->piece = deltaPiece; return true; }
    else { return false; }
}

bool Board::rotatePieceLeft()
{
    Piece deltaPiece = this->piece;

    deltaPiece.rotateLeft();

    if (this->canTransform(deltaPiece)) { this->piece = deltaPiece; return true; }
    else { return false; }
}

void Board::lowerActivePiece()
{
    if (this->isLanded())
    {
        pile.addPiece(this->piece);
        hasPiece = false;
    }
    else
    {
        this->piece.translate(0, 1);
    }
}

bool Board::isLanded()
{
    // dont get delta piece as we need not apply the transformation
    auto pieceShape = this->piece.getShape();
    auto pileShape = this->pile.getPile();

    int px = this->piece.getX();
    int py = this->piece.getY();

    for (int sy = 0; sy < pieceShape.size(); sy++)
    {
        for (int sx = 0; sx < pieceShape[sy].size(); sx++)
        {
            int x = sx + px; // global x coord
            int y = sy + py; // global y coord

            if (pieceShape[sy][sx] == 1)
            {
                if (!this->pile.isTileEmpty(x, y+1)) { return true; }
            }
        }
    }

    return false;
}

void Board::spawnPiece()
{
    this->piece = this->pieceFactory.getPiece();
}

void Board::calculateCellSize(const  int screenWidth, const  int screenHeight)
{
    int dw = screenWidth / this->BoardWidth;
    int dh = screenHeight / (this->BoardHeight - this->HiddenRows);

    // choose smaller
    if (dw < dh) { this->cellSize = dw; }
    else { this->cellSize = dh; }
}