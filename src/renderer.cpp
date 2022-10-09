#include "renderer.h"
#include "common.h"
#include "piece.h"
#include "pile.h"
#include "tetris.h"

#define STB_IMAGE_IMPLEMENTATION
#include <stb/stb_image.h>

#include <iostream>
#include <string>

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

Renderer::Renderer()
{
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

    // initialse resources
    this->initialiseResources();
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


void Renderer::initialiseResources()
{
    // load all images into arrays
    int w, h, comp; // only needed for stbi_load
    unsigned char* rawTileset = stbi_load(tilesetPath.c_str(), &w, &h, &comp, STBI_rgb);
    unsigned char* rawCharset = stbi_load(charsetPath.c_str(), &w, &h, &comp, STBI_rgb);
    unsigned char* rawBackgroundData = stbi_load(bgPath.c_str(), &w, &h, &comp, STBI_rgb);

    // ########## TILESET ##########
    for (unsigned int y = 0; y < palleteHeight; y++)
    {
        for (unsigned int x = 0; x < palleteWidth; x++)
        {
            // create pointers for source and dest
            unsigned char * src_ptr = rawTileset;
            unsigned char * dst_ptr = this->tilesetData[y][x];

            // move source pointer to location of first tile pixel
            src_ptr += (y * bpp * tileWidth * tileHeight * palleteWidth) + (x * bpp * tileWidth);

            // go row by row and copy from source into pallete vector
            for (unsigned int i = 0; i < tileHeight; i++)
            {
                memcpy(dst_ptr, src_ptr, bpp * tileWidth); // copy pixel row from src_ptr -> dst_ptr

                // move both pointers to next location
                src_ptr += bpp * tileWidth * palleteWidth; // down a row of pixels in source to get to next contiguos memory row
                dst_ptr += bpp * tileWidth; // down a row of pixels in dest
            }
        }
    }

    // ########## CHARSET ##########
    for (unsigned int i = 0; i < charTotal; i++)
    {
        // basically the same as above, assuming charset tiles are same dim as tileset
        unsigned char * src_ptr = rawCharset;
        unsigned char * dst_ptr = this->charsetData[i];

        src_ptr += i * tileWidth * bpp;

        for (unsigned int j = 0; j < tileHeight; j++)
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

void Renderer::clearBuffer()
{
    // copy background to buffer
    memcpy(buffer, backgroundData, bgWidth * bgHeight * bpp);
}

void Renderer::blitPlayfield(Pile const& pile, Piece const& activePiece, unsigned int level)
{
    Matrix renderTiles = pile.getMatrix();

    // add active piece to tiles
    if (activePiece.getType() != Tetromino::Empty)
    {
        Matrix pieceMatrix = activePiece.getMatrix();

        for (unsigned int y = 0; y < pieceMatrix.size(); y++)
        {
            for (unsigned int x = 0; x < pieceMatrix[y].size(); x++)
            {
                if (pieceMatrix[y][x] > 0 && activePiece.y >= 0)
                {
                    renderTiles[activePiece.y + y][activePiece.x + x] = pieceMatrix[y][x];
                }
            }
        }
    }

    unsigned int color = level % palleteHeight;
    unsigned int variant;

    const unsigned int playfieldStart = (playfield_y1 * bpp * bgWidth) + (playfield_x1 * bpp);

    for (int y = 0; y < PILE_HEIGHT - PILE_HIDDEN; y++)
    {
        for (int x = 0; x < PILE_WIDTH; x++)
        {
            if (renderTiles[y + PILE_HIDDEN][x] > 0) // Need to account for hidden row in pile coordinate system
            {
                variant = renderTiles[y + PILE_HIDDEN][x] - 1; //  Need to account for hidden row in pile coordinate system

                unsigned char * src_ptr = tilesetData[color][variant];
                unsigned char * dst_ptr = buffer;

                // move pointer to start of tile
                dst_ptr += playfieldStart + (y * bpp * tileHeight * bgWidth) + (x * bpp * tileWidth);

                for (unsigned int i = 0; i < tileHeight; i++)
                {
                    memcpy(dst_ptr, src_ptr, bpp * tileWidth);

                    src_ptr += bpp * tileWidth;
                    dst_ptr += bpp * bgWidth;
                }
            }

        }
    }
}


void Renderer::blitUI(Tetromino nextPiece, unsigned int level, unsigned int score, unsigned int topScore, unsigned int linesCleared, bool gameOver)
{
    // draw score
    drawNumString(score, 6, score_x1, score_y1);

    // draw topScore
    drawNumString(topScore, 6, score_x1, score_y1 - 24);

    // draw level
    drawNumString(level, 2, level_x1, level_y1);

    // draw lines cleared
    drawNumString(linesCleared, 3, cleared_x1, cleared_y1);

    if (gameOver)
    {
        // center of BG
        drawString("GAME OVER", bgWidth / 2 - 9 * 8 / 2 + 8, bgHeight / 2 - 8 / 2);
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

void Renderer::draw(unsigned int w, unsigned int h)
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

    for (unsigned int i = 0; i < tileHeight; i++)
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
    for (unsigned int i = 0; i < length - digits.size(); i++)
    {
        drawCharNumber(0, x1, y1);
        x1 += tileWidth;
    }

    // draw digits
    for (unsigned int i = 0; i < digits.size(); i++)
    {
        drawCharNumber(digits[i], x1, y1);
        x1 += tileWidth;
    }
}

void Renderer::drawChar(char c, unsigned int x1, unsigned int y1)
{
    // char to ascii to charset offset
    int ci = (int) c - 55;

    unsigned char * src_ptr = charsetData[ci];
    unsigned char * dst_ptr = buffer;

    dst_ptr += (y1 * bpp * bgWidth) + (x1 * bpp);

    for (unsigned int i = 0; i < tileHeight; i++)
    {
        memcpy(dst_ptr, src_ptr, bpp * tileWidth);

        src_ptr += bpp * tileWidth;
        dst_ptr += bpp * bgWidth;
    }
}

void Renderer::drawString(std::string str, unsigned int x1, unsigned int y1)
{
    for (char c: str)
    {
        if (c == ' ') { x1 += tileWidth; continue; }

        drawChar(c, x1, y1);

        x1 += tileWidth;
    }
}


void Renderer::drawNextPiece(Tetromino nextPiece, unsigned int level)
{
    const unsigned int nextStart = (next_y1 * bpp * bgWidth) + (next_x1 * bpp);

    unsigned int color = level % 10;
    unsigned int variant;

    Matrix nextPieceMatrix = getMatrixFromType(nextPiece);

    for (unsigned int y = 0; y < nextPieceMatrix.size(); y++)
    {
        for (unsigned int x = 0; x < nextPieceMatrix[y].size(); x++)
        {
            if (nextPieceMatrix[y][x] > 0)
            {
                variant = nextPieceMatrix[y][x] - 1;

                unsigned char * src_ptr = tilesetData[color][variant];
                unsigned char * dst_ptr = buffer;

                // move pointer to start of tile
                dst_ptr += nextStart + (y * bpp * tileHeight * bgWidth) + (x * bpp * tileWidth);

                for (unsigned int i = 0; i < tileHeight; i++)
                {
                    memcpy(dst_ptr, src_ptr, bpp * tileWidth);

                    src_ptr += bpp * tileWidth;
                    dst_ptr += bpp * bgWidth;
                }
            }
        }
    }
}