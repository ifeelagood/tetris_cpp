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