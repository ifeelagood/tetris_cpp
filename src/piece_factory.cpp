#include "piece.h"
#include "piece_factory.h"

#include <chrono>
#include <algorithm>


PieceFactory::PieceFactory()
{
    this->resetBag();
}

Piece PieceFactory::createPiece(const Shape& s, const Color& c) const
{
	Piece p = Piece(5, 2);
    p.setColor(c);
    p.setPieceShape(s);

    switch (s)
    {
        case Shape::L:
            p.setShape
            ({
                { 0,0,0 },
                { 1,1,1 },
                { 1,0,0 }
            });
            p.translate(-1, -1);
            break;

        case Shape::J:
            p.setShape
            ({
                {0,0,0},
                {1,1,1},
                {0,0,1}
            });
            p.translate(-1, -1);
            break;


        case Shape::T:
            p.setShape
            ({
                { 1,1,1 },
                { 0,1,0 },
                { 0,0,0 }
            });
            p.translate(-1, 0);
            break;

        case Shape::S:
            p.setShape
            ({
                { 0,0,0 },
                { 0,1,1 },
                { 1,1,0 }
            });
            p.translate(-1, -1);
            break;

        case Shape::Z:
            p.setShape
            ({
                { 0,0,0 },
                { 1,1,0 },
                { 0,1,1 }
            });
            break;

        case Shape::O:
            p.setShape
            ({
                { 0,0,0,0 },
                { 0,1,1,0 },
                { 0,1,1,0 },
                { 0,0,0,0 }
            });
            p.translate(-2, -1);
            break;

        case Shape::I:
            p.setShape
            ({
                { 0,0,0,0 },
                { 0,0,0,0 },
                { 1,1,1,1 },
                { 0,0,0,0 }
            });
            p.translate(-2, -1);
            break;

        case Shape::Empty:
            break;
    }

	return p;
}

Piece PieceFactory::getPiece()
{
    // get shape and remove from
    Shape s = this->bag.back();
    this->bag.pop_back();

    // check if we need another bag.
    this->checkBag();

    Color c;
    c.r = 1.0f; c.g = 1.0f; c.b = 0.0f;

    Piece p = this->createPiece(s, c);

    return p;
}

// bag functions
void PieceFactory::fillBag()
{
    // DO NOT RESIZE BAG.
    this->bag.push_back(Shape::L);
    this->bag.push_back(Shape::T);
    this->bag.push_back(Shape::Z);
    this->bag.push_back(Shape::S);
    this->bag.push_back(Shape::I);
    this->bag.push_back(Shape::J);
    this->bag.push_back(Shape::O);

}

void PieceFactory::shuffleBag()
{
    std::shuffle(this->bag.begin(), this->bag.end(), this->rng);
}

void PieceFactory::resetBag()
{
    this->bag.clear();
    this->fillBag();
    this->shuffleBag();
}

void PieceFactory::checkBag()
{
    if ((int) this->bag.size() == 0)
    {
        this->resetBag();
    }
}