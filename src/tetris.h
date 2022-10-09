#ifndef TETRIS_H
#define TETRIS_H

// Common types and constants
#include <stdint.h>
#include <vector>

// TYPES, ENUMS
typedef std::vector<std::vector<uint8_t>> Matrix; // matrix of cells
enum class Tetromino{L,T,Z,S,I,J,O,Empty}; // tetromino types

// CONSTANTS
#define PILE_WIDTH 10
#define PILE_HEIGHT 21
#define PILE_HIDDEN 1

// TEMPORARY CONSTANTS - these could be a setting
#define DAS_INITIAL_DELAY 16
#define DAS_REPEAT_DELAY 6

#endif // TETRIS_H