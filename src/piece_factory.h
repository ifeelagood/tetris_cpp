#pragma once

#include "piece.h"
#include <random>
#include <vector>


class PieceFactory
{
private:
    std::random_device rd;
    std::mt19937 rng{rd()};
    std::vector<Shape> bag;

    void init();

    Piece createPiece(const Shape& s, const Color& c) const;

    // fills bag without clearing, shuffling, or resizing
    void fillBag();

    // shuffle the bag
    void shuffleBag();

    // checks if bag is empty, then resizes, fills and shuffles if needed
    void checkBag();


public:
	PieceFactory();
	~PieceFactory() {};

    // interface for getting the next piece
    Piece getPiece();

    // reset the bag
    void resetBag();
};