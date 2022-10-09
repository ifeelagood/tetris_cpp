#ifndef PIECE_H
#define PIECE_H

#include <vector>
#include <iostream>
#include <map>

#include "tetris.h"

class Piece
{
private:
    Matrix matrix;
    Tetromino minoType;

public:
    int x, y; // position, dont need to be private.

    // constructors
    Piece() : minoType(Tetromino::Empty), x(0), y(0) {}; // for an empty piece
    Piece(Tetromino minoType); // constructs the piece from a tetromino type

    // getters
    Matrix getMatrix() const { return this->matrix; }
    Tetromino getType() const { return this->minoType; }

    // as rotation involves manipulation of the matrix, its better we have them as methods
    void rotateLeft();
    void rotateRight();
};

// we will use this in the constructor and rendering of "next" piece
Matrix getMatrixFromType(Tetromino minoType);


// static piece shapes
// The numbers in the shape represent the style of the cell
static const Matrix MATRIX_L =
{
    { 0,0,0 },
    { 2,2,2 },
    { 2,0,0 }
};

static const Matrix MATRIX_J =
{
    { 0,0,0 },
    { 3,3,3 },
    { 0,0,3 }
};

static const Matrix MATRIX_T =
{
    { 1,1,1 },
    { 0,1,0 },
    { 0,0,0 }
};

static const Matrix MATRIX_Z =
{
    { 0,0,0 },
    { 2,2,0 },
    { 0,2,2 }
};

static const Matrix MATRIX_S =
{
    { 0,0,0 },
    { 0,3,3 },
    { 3,3,0 }
};

static const Matrix MATRIX_I =
{
    { 0,0,0,0 },
    { 0,0,0,0 },
    { 1,1,1,1 },
    { 0,0,0,0 }
};

static const Matrix MATRIX_O =
{
    { 0,0,0,0 },
    { 0,1,1,0 },
    { 0,1,1,0 },
    { 0,0,0,0 }
};

static std::map<Tetromino, Matrix> MinoMatrix =
{
    {Tetromino::L, MATRIX_L},
    {Tetromino::J, MATRIX_J},
    {Tetromino::T, MATRIX_T},
    {Tetromino::Z, MATRIX_Z},
    {Tetromino::S, MATRIX_S},
    {Tetromino::I, MATRIX_I},
    {Tetromino::O, MATRIX_O},
};

// each tetromino has a unique starting position
static std::map<Tetromino, std::pair<int, int>> MinoPositions = 
{
    {Tetromino::L, {-1, -1}},
    {Tetromino::J, {-1, -1}},
    {Tetromino::T, {-1,  0}},
    {Tetromino::Z, {-1,  0}},
    {Tetromino::S, { 0,  0}},
    {Tetromino::I, {-2, -1}},
    {Tetromino::O, {-2, -1}},
};

#endif // PIECE_H