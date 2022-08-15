#pragma once

#include "piece.h"
#include <map>

// level, frames per gridcall
static std::map<unsigned int, unsigned int>LevelGravity = {
    {0, 48},
    {1, 43},
    {2, 38},
    {3, 33},
    {4, 28},
    {5, 23},
    {6, 18},
    {7, 13},
    {8, 8},
    {9, 6},
    {10, 5},
    {11, 5},
    {12, 5},
    {13, 4},
    {14, 4},
    {15, 4},
    {16, 3},
    {17, 3},
    {18, 3},
    {19, 2},
    {20, 2}
};

static std::map<unsigned int, unsigned int>LineScore = {
    {1, 40},
    {2, 100},
    {3, 300},
    {4, 1200}
};


class ColorRGB
{
public:
    float r,g,b;
    ColorRGB(float r, float g, float b) : r(r), g(g), b(b) {};
    ~ColorRGB() {};
};

// level % 6 to get color

static inline void getLevelColor(unsigned int level, float &r, float &g, float &b)
{
    unsigned int i = level % 6;

    switch(i)
    {
        case(0):
            r = 1.0f; g = 0.0f; b = 0.0f; break;
        case(1):
            r = 0.0f; g = 1.0f; b = 0.0f; break;
        case(2):
            r = 0.0f; g = 0.0f; b = 1.0f; break;
        case(3):
            r = 1.0f; g = 1.0f; b = 0.0f; break;
        case(4):
            r = 1.0f; g = 0.0f; b = 1.0f; break;
        case(5):
            r = 0.0f; g = 1.0f; b = 1.0f; break;
    }
}