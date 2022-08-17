#pragma once

#include "pile.h"
#include "piece.h"
#include "piece_factory.h"
#include "frametimer.h"
#include "keys.h"
#include "das.h"
#include "levels.h"
#include "ui.h"



class Board
{
private:
    // ##### VARIABLES #####

    // private objects
    Pile pile;
    PieceFactory pieceFactory;
    Piece piece;
    FrameTimer timer;
    FrameCounter counter;
    Keys oldKeys; // store to compare for press
    DAS das;
    Message msg;

    // game variables
    unsigned int level = 0;
    unsigned int score = 0;
    unsigned int topScore = 10000; // nes tetris default
    unsigned int gravity;
    unsigned int linesCleared;

    // dimensions and sizes
    const unsigned int HiddenRows = BOARD_HIDDEN;
    unsigned int BoardWidth, BoardHeight;
    unsigned int cellSize;

    // state variables
    bool quit = false;

    // ARE / entry delay
    unsigned int ARE = 0;

    // ##### FUNCTIONS #####

    // COLLISION FUNCTIONS
    bool canTransform(Piece const &deltaPiece);
    bool checkPileCollision(Piece const &p);
    bool outOfBounds(Piece const &p);
    bool isLanded();

    // TRANSFORMATION FUNCTIONS
    bool translatePiece(const int dx, const int dy);
    bool rotatePieceRight();
    bool rotatePieceLeft();
    void lowerActivePiece();

    // MISC GAME FUNCTIONS
    void resetGame();
    void levelUp();
    void spawnPiece();
    void calculateARE(int row);



public:
    // CONSTRUCTORS
    Board(const int w, const int h);
    // ~Board() {};

    // MAIN GAME LOOP
    void update(Keys const &keys);

    // GETTERS
    int getBoardWidth()  const { return this->BoardWidth; }
    int getBoardHeight() const { return this->BoardHeight; }
    Piece getPiece() const { return this->piece; } // return current playing piece
    unsigned int getCellSize() const { return this->cellSize; }

    unsigned int getScore() const { return this->score; }
    unsigned int getLevel() const { return this->level; }
    unsigned int getTopScore() const { return this->topScore; }
    unsigned int getLinesCleared() const { return this->linesCleared; }

    float getFPS() const { return this->timer.getFPS(); }
    Message getMessage() const { return this->msg; }

    // MISC
    bool isTileEmpty(int x, int y) { return this->pile.isTileEmpty(x, y); };
    std::vector<std::vector<int>> getPileShape() { return this->pile.getPile(); }

    void calculateCellSize(const int screenWidth, const int screenHeight);

    void quitGame() { this->quit = true; }

};