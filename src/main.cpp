#include <cstdio>
#include <iostream>
#include <chrono>
#include <thread>

#if defined(_WIN32) || defined(_WIN64)
#include <Windows.h>
#endif

#include <GL/freeglut_std.h>
#include <GL/gl.h>
#include <GL/glu.h>

#include <string>
#include <type_traits>

#include "main.h"
#include "board.h"
#include "frametimer.h"
#include "keys.h"
#include "levels.h"
#include "stb_image.h"
#include "textures.h"

Board game(BOARD_WIDTH, BOARD_HEIGHT); // 2 hidden rows
Keys keys;


TileTexture tileTex;
CharsetTextures charTex;
unsigned int bgTex;

// keyboard functions

void keyboardNormalUp(unsigned char key, int x, int y)
{
    if (key == 'z') { keys.z = 0; }
    if (key == 'x') { keys.x = 0; }
}

void keyboardNormalDown(unsigned char key, int x, int y)
{
    if (key == 'z') { keys.z = 1; }
    if (key == 'x') { keys.x = 1; }
}

void keyboardSpecialUp(int key, int x, int y)
{
    if (key == GLUT_KEY_LEFT ) { keys.left  = 0; }
    if (key == GLUT_KEY_RIGHT) { keys.right = 0; }
    if (key == GLUT_KEY_DOWN ) { keys.down  = 0; }
}

void keyboardSpecialDown(int key, int x, int y)
{
    if (key == GLUT_KEY_LEFT ) { keys.left  = 1; }
    if (key == GLUT_KEY_RIGHT) { keys.right = 1; }
    if (key == GLUT_KEY_DOWN ) { keys.down  = 1; }
}

// window and display functions
void windowSize(int w, int h)
{
    glViewport(0, 0, w, h);
    // glLoadIdentity();
    // gluOrtho2D(0, w, h, 0);
}


// drawing functions

void drawTexturedQuad(unsigned int &texture, int x1, int x2, int y1, int y2)
{
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, texture);

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


void drawString(int x, int y, void *font, const char* string)
{
    const unsigned char* c = reinterpret_cast<const unsigned char*>(string);

    glColor3f(1.0f, 1.0f, 1.0f);
    glRasterPos2i(x, y);

    glutBitmapString(font, c);
}

void drawUI()
{

}

void drawBackground()
{
    int x1, x2, y1, y2;
    x1 = 0; x2 = WINDOW_WIDTH;
    y1 = 0; y2 = WINDOW_HEIGHT;

    drawTexturedQuad(bgTex, x1, x2, y1, y2);
}

void drawCell(int cellX, int cellY, int variation)
{
    int cellSize = TILE_WIDTH;

    int x = cellX * cellSize;
    int y = (cellY - 2) * cellSize;

    int x1, x2, y1, y2;
    x1 = x + PF_X1; x2 = x1 + cellSize;
    y1 = y + PF_Y1; y2 = y1 + cellSize;

    unsigned int color = game.getLevel() % 9; // 10 colors in the pallete
    unsigned int texture = tileTex.getTexture(color, variation);

    drawTexturedQuad(texture, x1, x2, y1, y2);

}

void drawPile()
{
    auto shape = game.getPileShape();

    for (int y = 0; y < shape.size(); y++)
    {
        for (int x = 0; x < shape[y].size(); x++)
        {
            if (shape[y][x] > 0) { drawCell(x, y, shape[y][x] - 1); }
        }
    }
}

void drawActivePiece()
{
    Piece p = game.getPiece();

    if (p.getPieceShape() != Shape::Empty)
    {
        auto shape = p.getShape();

        int px = p.getX();
        int py = p.getY();


        for (int y = 0; y < shape.size(); y++)
        {
            for (int x = 0; x < shape[y].size(); x++)
            {
                // std::cout << shape[y][x];
                if (shape[y][x] > 0) { drawCell(px+x, py+y, shape[y][x]-1); }
            }
        }
    }
}

void resize(const int w, const int h)
{
    glutReshapeWindow(WINDOW_WIDTH, WINDOW_HEIGHT);
    // windowSize(w, h);
    // game.calculateCellSize(w, h);
}


std::chrono::system_clock::time_point a = std::chrono::system_clock::now();
std::chrono::system_clock::time_point b = std::chrono::system_clock::now();

void fpsLimit()
{
    // ###########THE GREAT FPS LIMITER##########################################################################
    a = std::chrono::system_clock::now();                                                                    // #
    std::chrono::duration<double, std::milli> work_time = a - b;                                             // #
                                                                                                             // #
    double target = ((1.0 / (double) FPS) * 1000.0);                                                         // #
    if (work_time.count() < target)                                                                          // #
    {                                                                                                        // #
        std::chrono::duration<double, std::milli> delta_ms(target - work_time.count());
        auto delta_ms_duration = std::chrono::duration_cast<std::chrono::milliseconds>(delta_ms);
        std::this_thread::sleep_for(std::chrono::milliseconds(delta_ms_duration.count()));
    }

    b = std::chrono::system_clock::now();
    // std::chrono::duration<double, std::milli> sleep_time = b - a;
    // #########################################################################################################
}

// display function
void display()
{
    fpsLimit();

    const int width = glutGet(GLUT_WINDOW_WIDTH);
    const int height = glutGet(GLUT_WINDOW_HEIGHT);

    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0, width, height, 0, -1, 1);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    windowSize(width, height);

    drawBackground();
    drawPile();
    drawActivePiece();
    drawUI();

    game.update(keys);

    glutSwapBuffers();

    // printf("Time: %f \n", (work_time + sleep_time).count());
    glutPostRedisplay();
}



// initialisation
void init(int* pargc, char** argv)
{
    // initialise glut
    glutInit(pargc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    glutCreateWindow("Tetris");
    glutInitWindowSize(WINDOW_WIDTH, WINDOW_HEIGHT);

    // set glut callback functions
    glutDisplayFunc(display);
    glutKeyboardUpFunc(keyboardNormalUp);
    glutSpecialUpFunc(keyboardSpecialUp);
    glutKeyboardFunc(keyboardNormalDown);
    glutSpecialFunc(keyboardSpecialDown);
    glutReshapeFunc(resize);

    std::printf("Initialised GLUT\n");

    // get and print rendering device
    const char * gl_renderer = (const char *) glGetString(GL_RENDERER);
    std::printf("Using device: '%s\n'", gl_renderer);

    // calculate tile size
    game.calculateCellSize(WINDOW_WIDTH, WINDOW_HEIGHT);

    // generate textures
    tileTex.generateTextures();
    charTex.generateTextures();
    createBGTexture(bgTex);
}

// main
int main(int argc, char** argv)
{
    init(&argc, argv);

    // start glut display callback function
    glutMainLoop();
}