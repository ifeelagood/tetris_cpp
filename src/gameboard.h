#pragma once

#include "pile.h"
#include "piece.h"
#include "piece_factory.h"
#include "frametimer.h"
#include "keys.h"
#include "das.h"
#include "levels.h"



class GameBoard
{
private:
    // private objects
    Pile pile;
    PieceFactory pieceFactory;
    Piece piece;
    FrameTimer timer;
    FrameCounter counter;
    Keys oldKeys; // store to compare for single activation
    DAS das;

    // tetris level, gravity. refer to nes tetris wiki.
    unsigned int level = 0;

    unsigned int gravity;
    unsigned int linesCleared;

    // width and height.
    int w, h;

    unsigned int cellSize;

    bool canTransform(Piece const &deltaPiece);

    // determines if piece is colliding with the pile
    bool checkPileCollision(Piece const &p);

    // checsk whether out of bounds
    bool outOfBounds(Piece const &p);

    // translate active piece with checking for out of bound. returns false if unsuccessful
    bool translatePiece(const int dx, const int dy);

    // rotates active piece
    // TODO and tspins
    bool rotatePieceRight();
    bool rotatePieceLeft();

    void lowerActivePiece();

    // level up
    void levelUp();
    // spawn piece
    void spawnPiece();

    void resetGame();

    // temporary
    bool hasPiece = false;

    // hard quit
    bool quit = false;

    // losing
    bool gameEnded = false;

public:
    // constructors
    GameBoard(const int w, const int h);
    ~GameBoard() {};

    // main game loop
    void gameLoop(Keys const &keys);

    // calculates cell size based on
    void calculateCellSize(const unsigned int screenWidth, const unsigned int screenHeight);

    // getters
    int getCellWidth()  const { return this->w; }
    int getCellHeight() const { return this->h; }
    Piece getPiece() const { return this->piece; } // return current playing piece
    unsigned int getCellSize() const { return this->cellSize; }

    bool isTileEmpty(int x, int y) { return this->pile.isTileEmpty(x, y); };

    // quit the program
    bool isPlaying() const { return !(this->quit); }

    void quitGame() { this->quit = true; }

    // on keyboard
    void update(Keys const &keys);

    // checks if active piece has landed on floor or pile
    bool isLanded();

    bool isToppedOut(); // checks if we are topped out
};