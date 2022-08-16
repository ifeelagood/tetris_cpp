#include "textures.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"

// opengl headers for textures and such.
#include <GL/gl.h>

#include <iostream>
#include <string>


TileTexture::TileTexture()
{
    this->allocatePallete(); // allocate memory for the pallete
    this->rawData = stbi_load("res/blocktiles.png", &this->w, &this->h, &this->bpp, STBI_rgb); // load raw pallete into memory
    this->fillTileData(); // fill pallete vector
    // this->generateTextures();
}

TileTexture::~TileTexture()
{
    if (!this->isFree)
    {
        this->deallocatePallete(); // free pallete allocation
        stbi_image_free(this->rawData); // free raw image memory. Should we do this earlier?
    }
}

void TileTexture::allocatePallete()
{
    // resize pallete
    this->pallete.resize(this->ph);
    for (int y = 0; y < this->ph; y++)
    {
        this->pallete[y].resize(this->pw);
    }

    // allocate pallete
    for (int y = 0; y < this->ph; y++)
    {
        for (int x = 0; x < this->pw; x++)
        {
            this->pallete[y][x] = new unsigned char[this->ps * this->ps * this->pbpp];
        }
    }

}

void TileTexture::deallocatePallete()
{
    for (int y = 0; y < this->ph; y++)
    {
        for (int x = 0; x < this->pw; x++)
        {
            delete[] this->pallete[y][x];
        }
    }
}


void TileTexture::fillTileData()
{
    // ommiting this-> for clarity
    // including comments for clarity
    // ^ yes i googled opposite of omission

    // define confusing constants
    for (int y = 0; y < 10; y++)
    {
        for (int x = 0; x < 4; x++)
        {
            // create pointers for source and dest
            unsigned char* src_ptr = this->rawData;
            unsigned char* dst_ptr = this->pallete[y][x];

            // move source pointer to location of first tile pixel
            // this is exactly (pallete y * bytes per source row) + (pallete x * bytes per tile row)
            src_ptr += (y * pw * ps * ps * pbpp) + (x * pbpp * ps);

            // go row by row and copy from source into pallete vector
            // must do this as source pixel rows are not contiguous in memory
            for (int i = 0; i < ps; i++)
            {
                memcpy(dst_ptr, src_ptr, pbpp * ps); // copy pixel row from src_ptr -> dst_ptr

                // move both pointers to next location
                src_ptr += pbpp * ps * pw; // down a row of pixels in source to get to next contiguos memory row
                dst_ptr += pbpp * ps; // down a row of pixels in dest
            }

            // std::string filepath = "res/" + std::to_string(x) + '_' + std::to_string(y) + ".png";
            // stbi_write_png(filepath.c_str(), ps, ps, pbpp, this->pallete[y][x], pbpp * ps);
        }
    }
}

void TileTexture::generateTextures()
{
    textures.resize(ph);
    for (int y = 0; y < ph; y++)
    {
        textures[y].resize(pw);
    }

    for (int y = 0; y < ph; y++)
    {
        for (int x = 0; x < pw; x++)
        {
            unsigned char * data = this->pallete[y][x];

            // generate with opengl
            glGenTextures(1, &this->textures[y][x]);
            glBindTexture(GL_TEXTURE_2D, this->textures[y][x]);

            // IMPORTANT texture filtering
            // if we use (why we?) bilinear filtering, it'll look like shit, so just nearest
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER); // we dont want any of that GL_REPEAT bs
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST); // dont bother with mipmaps
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

            // generate texture
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, ps, ps, 0, GL_RGB, GL_UNSIGNED_BYTE, data);


            // unbind texture
            glBindTexture(GL_TEXTURE_2D, 0);
        }
    }

    // now free raw data and pallete data
    stbi_image_free(this->rawData);
    this->deallocatePallete();
    isFree = true;

}

// charset

CharsetTextures::CharsetTextures()
{
    // resize charset vector and allocate
    this->charset.resize(this->total);
    for (int i = 0; i < this->total; i++) { this->charset[i] = new unsigned char[w * h * bpp]; }


    // load raw data
    int raw_w, raw_h, raw_bpp;
    this->rawData = stbi_load("res/charset.png", &raw_w, &raw_h, &raw_bpp, STBI_rgb);

    this->splitRawData();
}

// split 1D array of pixels into individual 1D arrays
void CharsetTextures::splitRawData()
{
    const int rowLength = bpp * total * w;

    for (int i = 0; i < this->total; i++)
    {
        this->charset[i] = new unsigned char[w * h * bpp];

        unsigned char* src_ptr = this->rawData;
        unsigned char* dst_ptr = this->charset[i];

        // move pointer to first pixel of character
        src_ptr += i * (w * bpp);

        for (int y = 0; y < h; y++)
        {
            memcpy(dst_ptr, src_ptr, w * bpp);

            src_ptr += bpp * total * w;
            dst_ptr += bpp * w;
        }
    }

}

void CharsetTextures::generateTextures()
{
    // resize texture vector
    this->textures.resize(this->total);

    for (int i = 0; i < this->total; i++)
    {
        unsigned char * data = this->charset[i];

        // generate with opengl
        glGenTextures(1, &this->textures[i]);
        glBindTexture(GL_TEXTURE_2D, this->textures[i]);

        // IMPORTANT texture filtering
        // if we use (why we?) bilinear filtering, it'll look like shit, so just nearest
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER); // we dont want any of that GL_REPEAT bs
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST); // dont bother with mipmaps
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

        // generate texture
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, w, h, 0, GL_RGB, GL_UNSIGNED_BYTE, data);


        // unbind texture
        glBindTexture(GL_TEXTURE_2D, 0);

        delete[] this->charset[i];
    }

    stbi_image_free(this->rawData);
}

void createBGTexture(unsigned int &texture)
{
    int w, h, bpp;
    unsigned char * data = stbi_load("res/background.png", &w, &h, &bpp, STBI_rgb);


    // generate with opengl
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);

    // IMPORTANT texture filtering
    // if we use (why we?) bilinear filtering, it'll look like shit, so just nearest
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER); // we dont want any of that GL_REPEAT bs
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST); // dont bother with mipmaps
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    // generate texture
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB,w, h, 0, GL_RGB, GL_UNSIGNED_BYTE, data);


    // unbind texture
    glBindTexture(GL_TEXTURE_2D, 0);
}