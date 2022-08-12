#pragma once

#include <vector>

enum class Shape{L,T,Z,S,I,J,O,Empty};


struct Color
{
    float r,g,b;
};

class Piece
{
private:
    int id;
    int x, y;
    int w, h;
    Color color;

    std::vector<std::vector<int>> shape;
    Shape pieceShape;
    static int IDs;

public:
    // constructors
    Piece() : id(generateId()) {};
    Piece(const int x, const int y) : id(generateId()), x(x), y(y) {};

    // operator assignment
    Piece& operator=(Piece const &p2);

    // getters
    int getId() const { return this->id; }
    int getX()  const { return this->x; }
    int getY()  const { return this->y; }
    std::vector<std::vector<int>> getShape() const { return this->shape; }
    Shape getPieceShape() const { return this->pieceShape; }
    Color getColor() const { return this->color; }

    // setters
    void setShape(std::vector<std::vector<int>> const &newShape) { this->shape = newShape; }
    void setPieceShape(Shape const &pieceShape ) { this->pieceShape = pieceShape; }
    void setX(const int x) { this->x = x; }
    void setY(const int y) { this->y = y; }
    void setColor(Color newColor) { this->color = newColor; }

    // id generator
    static int generateId() { IDs++; return IDs; };

    // translation and rotation
    void rotateRight();
    void rotateLeft();

    void translate(int dx, int dy);
};