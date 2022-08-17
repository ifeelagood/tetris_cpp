#include "renderer.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

// opengl headers for textures and such.
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/freeglut_std.h>

#include <iostream>
#include <string>

#include "main.h"

Renderer::Renderer()
{
    // ############# ALLOCATION ############################
    // allocate raw buffer and bg
    buffer = new unsigned char[bpp * bgWidth * bgHeight];
    backgroundData = new unsigned char[bpp * bgWidth * bgHeight];

    // resize vectors
    tilesetData.resize(palleteHeight);
    charsetData.resize(charTotal);
    for (int i = 0; i < palleteHeight; i++) { tilesetData[i].resize(palleteWidth); }

    // allocate vectors
    for (int i = 0; i < charTotal; i++)  { charsetData[i] = new unsigned char[bpp * tileWidth * tileHeight]; }

    for (int y = 0; y < palleteHeight; y++)
    {
        for (int x = 0; x < palleteWidth; x++)
        {
            tilesetData[y][x] = new unsigned char[bpp * tileWidth * tileHeight];
        }
    }

    // initialse data vectors
    this->initData();
}

Renderer::~Renderer()
{
    // deallocate buffer and bg
    delete[] buffer;
    delete[] backgroundData;

    for (int i = 0; i < charTotal; i++) { delete[] charsetData[i]; }

    for (int y = 0; y < palleteHeight; y++)
    {
        for (int x = 0; x < palleteWidth; x++)
        {
            delete[] tilesetData[y][x];
        }
    }
}

void Renderer::initData()
{
    // load all images into arrays
    int w, h, comp; // only needed for stbi_load
    unsigned char* rawTileset = stbi_load(tilesetPath.c_str(), &w, &h, &comp, STBI_rgb);
    unsigned char* rawCharset = stbi_load(charsetPath.c_str(), &w, &h, &comp, STBI_rgb);
    unsigned char* rawBackgroundData = stbi_load(bgPath.c_str(), &w, &h, &comp, STBI_rgb);

    // ########## TILESET ##########
    for (int y = 0; y < palleteHeight; y++)
    {
        for (int x = 0; x < palleteWidth; x++)
        {
            // create pointers for source and dest
            unsigned char * src_ptr = rawTileset;
            unsigned char * dst_ptr = this->tilesetData[y][x];

            // move source pointer to location of first tile pixel
            src_ptr += (y * bpp * tileWidth * tileHeight * palleteWidth) + (x * bpp * tileWidth);

            // go row by row and copy from source into pallete vector
            for (int i = 0; i < tileHeight; i++)
            {
                memcpy(dst_ptr, src_ptr, bpp * tileWidth); // copy pixel row from src_ptr -> dst_ptr

                // move both pointers to next location
                src_ptr += bpp * tileWidth * palleteWidth; // down a row of pixels in source to get to next contiguos memory row
                dst_ptr += bpp * tileWidth; // down a row of pixels in dest
            }
        }
    }

    // ########## CHARSET ##########
    for (int i = 0; i < charTotal; i++)
    {
        // basically the same as above, assuming charset tiles are same dim as tileset
        unsigned char * src_ptr = rawCharset;
        unsigned char * dst_ptr = this->charsetData[i];

        src_ptr += i * tileWidth * bpp;

        for (int j = 0; j < tileHeight; j++)
        {
            memcpy(dst_ptr, src_ptr, bpp * tileWidth);

            src_ptr += charTotal * bpp * tileWidth;
            dst_ptr += bpp * tileWidth;
        }
    }

    // ######### BACKGROUND ##########
    memcpy(this->backgroundData, rawBackgroundData, bgWidth * bgHeight * bpp); // do this to control freeing


    // ######### FREE MEM ##########
    stbi_image_free(rawTileset);
    stbi_image_free(rawCharset);
    stbi_image_free(rawBackgroundData);
}

void Renderer::render(unsigned int windowWidth, unsigned int windowHeight, std::vector<std::vector<int>> const &pile, Piece const &piece, unsigned int level, unsigned int score, unsigned int topScore, unsigned int linesCleared)
{
    std::vector<std::vector<int>> tiles = pile;


    // remove hidden rows
    // tiles.erase(tiles.begin(), tiles.begin() + BOARD_HIDDEN);

    // add piece to tiles
    if (piece.getPieceShape() != Shape::Empty)
    {
        auto pieceTiles = piece.getShape();

        for (int y = 0; y < pieceTiles.size(); y++)
        {
            for (int x = 0; x < pieceTiles[y].size(); x++)
            {
                int px = x + piece.getX();
                int py = y + piece.getY();

                if (pieceTiles[y][x] > 0 && py >= 0)
                {
                    tiles[py][px] = pieceTiles[y][x];
                }
            }
        }
    }

    // blit tiles to screen
    drawTiles(tiles, level);

    // draw score
    drawNumString(score, 6, score_x1, score_y1);

    // draw topScore
    drawNumString(topScore, 6, score_x1, score_y1 - 24);

    // draw level
    drawNumString(level, 2, level_x1, level_y1);

    // draw lines cleared
    drawNumString(linesCleared, 3, cleared_x1, cleared_y1);

    // generate tex
    generateTexture();

    // draw it
    drawTexture(windowWidth, windowHeight);
}

void Renderer::drawTiles(std::vector<std::vector<int>> const &tiles, unsigned int level)
{
    // copy background to buffer
    memcpy(buffer, backgroundData, bgWidth * bgHeight * bpp);

    unsigned int color = level % palleteHeight;
    unsigned int variant;

    const unsigned int playfieldStart = (playfield_y1 * bpp * bgWidth) + (playfield_x1 * bpp);

    // dont draw board hidden
    for (int y = 0; y < BOARD_HEIGHT - BOARD_HIDDEN; y++)
    {
        for (int x = 0; x < BOARD_WIDTH; x++)
        {
            if (tiles[y+1][x] > 0) // y + 1 is tile array position
            {
                variant = tiles[y+1][x] - 1; // y + 1 is tile array position

                unsigned char * src_ptr = tilesetData[color][variant];
                unsigned char * dst_ptr = buffer;

                // move pointer to start of tile
                dst_ptr += playfieldStart + (y * bpp * tileHeight * bgWidth) + (x * bpp * tileWidth);

                for (int i = 0; i < tileHeight; i++)
                {
                    memcpy(dst_ptr, src_ptr, bpp * tileWidth);

                    src_ptr += bpp * tileWidth;
                    dst_ptr += bpp * bgWidth;
                }
            }
        }
    }
}

void Renderer::generateTexture()
{

    glGenTextures(1, &tex);
    glBindTexture(GL_TEXTURE_2D, tex);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER); // we dont want any of that GL_REPEAT bs
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, bgWidth, bgHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, buffer);
    glBindTexture(GL_TEXTURE_2D, 0);
}

void Renderer::drawTexture(unsigned int w, unsigned int h)
{
    int x1 = 0; int x2 = w;
    int y1 = 0; int y2 = h;

    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, tex);

    glColor3f(1.0f, 1.0f, 1.0f);
    glBegin(GL_QUADS);
        glTexCoord2i(0, 0); glVertex2i(x1, y1);
        glTexCoord2i(1, 0); glVertex2i(x2, y1);
        glTexCoord2i(1, 1); glVertex2i(x2, y2);
        glTexCoord2i(0, 1); glVertex2i(x1, y2);
    glEnd();

    glBindTexture(GL_TEXTURE_2D, 0);
    glDisable(GL_TEXTURE_2D);
}

void Renderer::drawCharNumber(unsigned int num, unsigned int x1, unsigned int y1)
{
    unsigned char * src_ptr = charsetData[num];
    unsigned char * dst_ptr = buffer;

    dst_ptr += (y1 * bpp * bgWidth) + (x1 * bpp);

    for (int i = 0; i < tileHeight; i++)
    {
        memcpy(dst_ptr, src_ptr, bpp * tileWidth);

        src_ptr += bpp * tileWidth;
        dst_ptr += bpp * bgWidth;
    }
}


void collect_digits(std::vector<int>& digits, unsigned long num) {
    if (num > 9) {
        collect_digits(digits, num / 10);
    }
    digits.push_back(num % 10);
}

void Renderer::drawNumString(unsigned int num, unsigned int length, unsigned int x1, unsigned int y1)
{

    std::vector<int> digits;
    collect_digits(digits, num);

    // draw leading zeros
    for (int i = 0; i < length - digits.size(); i++)
    {
        drawCharNumber(0, x1, y1);
        x1 += tileWidth;
    }

    // draw digits
    for (int i = 0; i < digits.size(); i++)
    {
        drawCharNumber(digits[i], x1, y1);
        x1 += tileWidth;
    }
}