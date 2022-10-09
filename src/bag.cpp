#include "bag.h"

#include <algorithm>

// BAG

Bag::Bag()
{
    this->fill();
    this->shuffle();
}

void Bag::fill()
{
    this->bag.push_back(Tetromino::L);
    this->bag.push_back(Tetromino::T);
    this->bag.push_back(Tetromino::Z);
    this->bag.push_back(Tetromino::S);
    this->bag.push_back(Tetromino::I);
    this->bag.push_back(Tetromino::J);
    this->bag.push_back(Tetromino::O);
}

void Bag::shuffle()
{
    std::shuffle(this->bag.begin(), this->bag.end(), this->rng);
}

Tetromino Bag::getNext()
{
    Tetromino s = this->bag.back();
    this->bag.pop_back();

    if (this->isEmpty()) { this->fill(); this->shuffle(); } // refill and shuffle if empty


    return s;
}

Tetromino Bag::peekNext()
{
    return this->bag.back();
}

void Bag::reset()
{
    this->bag.clear();
    this->fill();
    this->shuffle();
}
