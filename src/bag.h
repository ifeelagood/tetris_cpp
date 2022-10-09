#ifndef BAG_H
#define BAG_H

// Bag of tetrominos

#include <random>
#include <vector>

#include "piece.h" // for the enum

class Bag
{
private:
    std::random_device rd;
    std::mt19937 rng{rd()};

    std::vector<Tetromino> bag;

    void fill();
    void shuffle();

    bool isEmpty() const { return (this->bag.size() == 0); }

public:
    Bag();
    ~Bag() {};

    Tetromino getNext(); // removes from bag
    Tetromino peekNext(); // does not remove from bag

    void reset(); // clears bag and refills
};

#endif // BAG_H