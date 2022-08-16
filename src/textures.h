#pragma once

#include <vector>

class TileTexture
{
private:

    bool isFree = false; // has data been freed

    int w, h, bpp; // vars for stbi_load

    const int pw = 4; // pallete width (i.e. tile variations)
    const int ph = 10; // pallete height (i.e. tile colors)
    const int ps = 8; // pallete size (i.e. pixels per tile)
    const int pbpp = 3; // pallete bits per pixel

    unsigned char *rawData;
    void allocatePallete(); // resize pallete array and allocate
    void deallocatePallete(); // delete pallete data
    void fillTileData(); // fill pallete vector

    std::vector<std::vector<unsigned int>> textures;
    std::vector<std::vector<unsigned char *>> pallete; // 2d vector of raw block data as unsigned char*



public:
    TileTexture();
    ~TileTexture();

    // MUST BE CALLED AFTER GL INIT
    void generateTextures(); // generate all opengl textures

    // row->color; col->variant; returns opengl texture id
    unsigned int getTexture(unsigned int row, unsigned int col)  { return this->textures[row][col]; }

};