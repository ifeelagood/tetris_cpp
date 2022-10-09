#pragma once

#include <vector>
#include <string>
#include <map>

#include "piece.h" // getting matrix from type
#include "pile.h"
#include "tetris.h"

#define TILE_SIZE 8
#define BG_WIDTH 8
#define BG_HEIGHT 7


class Renderer
{
private:
    // start pixel of playfield in background
    const unsigned int playfield_x1 = 96;
    const unsigned int playfield_y1 = 40;

    // start pixel of score
    const unsigned int score_x1 = 192;
    const unsigned int score_y1 = 56;

    // start pixel of top score
    const unsigned int topscore_x1 = 192;
    const unsigned int topscore_y1 = 32;

    // start pixel of level
    const unsigned int level_x1 = 208;
    const unsigned int level_y1 = 160;

    // start pixel of lines cleared
    const unsigned int cleared_x1 = 152;
    const unsigned int cleared_y1 = 16;

    // start pixel of next piece
    const unsigned int next_x1 = 192;
    const unsigned int next_y1 = 104;


    // tile dimensions in pixels
    const unsigned int tileWidth = 8;
    const unsigned int tileHeight = 8;

    // pallete width in tiles
    const unsigned int palleteWidth = 4;
    const unsigned int palleteHeight = 10;

    // background width in pixels
    const unsigned int bgWidth = 256;
    const unsigned int bgHeight = 224;


    // amount of chars in charset
    const unsigned int charTotal = 36;

    const unsigned int bpp = 3; // 3 bits per pixel

    // buffer and background
    unsigned char *buffer;
    unsigned char *backgroundData;

    // vectors for charset and tile pixel data
    std::vector<std::vector<unsigned char *>> tilesetData;
    std::vector<unsigned char *> charsetData;

    // image paths
    std::string bgPath = "res/background.png";
    std::string tilesetPath = "res/tileset.png";
    std::string charsetPath = "res/charset.png";

    unsigned int tex; // opengl texture

    // load and split tiles and charset into vectors
    void initialiseResources();

    void drawTiles(std::vector<std::vector<unsigned char>> const &tiles, unsigned int level);


    void drawCharNumber(unsigned int num, unsigned int x, unsigned int y);
    void drawChar(char c, unsigned int x, unsigned int y);

    void drawNumString(unsigned int num, unsigned int length, unsigned int x1, unsigned int y1);
    void drawString(std::string str, unsigned int x, unsigned int y);

    void drawNextPiece(Tetromino nextPiece, unsigned int level);

public:

    Renderer();
    ~Renderer();

    // void render(unsigned int windowWidth, unsigned int windowHeight, std::vector<std::vector<unsigned char>> const &pile, Piece const &piece, Piece const &nextPiece, unsigned int level, unsigned int score, unsigned int topScore, unsigned int linesCleared, bool gameOver);

    void clearBuffer(); // replaces buffer with background
    void blitPlayfield(Pile const& pile, Piece const& activePiece, unsigned int level);
    void blitUI(Tetromino nextPiece, unsigned int level, unsigned int score, unsigned int topScore, unsigned int linesCleared, bool gameOver);
    void generateTexture();
    void draw(unsigned int w, unsigned int h);
};