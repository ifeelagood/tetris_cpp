#include <cstdio>
#include <iostream>
#include <chrono>
#include <thread>

#include <GL/freeglut_std.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>
#include <string>
#include <type_traits>

#include "main.h"
#include "board.h"
#include "frametimer.h"
#include "keys.h"
#include "levels.h"
#include "stb_image.h"
#include "renderer.h"

Board game(BOARD_WIDTH, BOARD_HEIGHT); // 2 hidden rows
Keys keys;


Renderer renderer;


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

    game.update(keys);
    renderer.render(width, height, game.getPileShape(), game.getPiece(), game.getLevel(), game.getScore(), game.getTopScore(), game.getLinesCleared());

    glutSwapBuffers();

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
}

// main
int main(int argc, char** argv)
{
    init(&argc, argv);

    // start glut display callback function
    glutMainLoop();
}