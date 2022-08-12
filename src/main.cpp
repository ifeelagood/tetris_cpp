#include <GL/freeglut_std.h>
#include <iostream>

#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>

#include "main.h"
#include "gameboard.h"
#include "frametimer.h"
#include "keys.h"

GameBoard game(BOARD_WIDTH, BOARD_HEIGHT+2); // 2 hidden rows
Keys keys;

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
// drawing functions

void helpimretarded()
{
    const int w = glutGet(GLUT_WINDOW_WIDTH);
    const int h = glutGet(GLUT_WINDOW_HEIGHT);

    int x1, y1, x2, y2;
    x1 = 0; x2 = w;
    y1 = 0; y2 = h;

    glColor3f(0.0f, 1.0f, 0.0f);
    glBegin(GL_QUADS);
        glVertex2i(x1, y1);
        glVertex2i(x2, y1);
        glVertex2i(x2, y2);
        glVertex2i(x1, y2);
    glEnd();
}

void drawBackground()
{
    int cellSize = (int) game.getCellSize();

    int x1, x2, y1, y2;
    x1 = 0; x2 = cellSize * BOARD_WIDTH;
    y1 = 0; y2 = cellSize * BOARD_HEIGHT;


    glColor3f(0.3f, 0.3f, 0.3);
    glBegin(GL_QUADS);
        glVertex2i(x1, y1);
        glVertex2i(x2, y1);
        glVertex2i(x2, y2);
        glVertex2i(x1, y2);
    glEnd();
}

void drawCell(int cellX, int cellY)
{
    int cellSize = (int) game.getCellSize();

    int x = cellX * cellSize;
    int y = (cellY - 2) * cellSize;

    int x1, x2, y1, y2;
    x1 = x; x2 = x + cellSize;
    y1 = y; y2 = y + cellSize;

    glColor3f(1.0f, 0.0f, 1.0f);
    glBegin(GL_QUADS);
        glVertex2i(x1, y1);
        glVertex2i(x2, y1);
        glVertex2i(x2, y2);
        glVertex2i(x1, y2);
    glEnd();
}


void drawPile()
{
    for (int y = 0; y < game.getCellHeight(); y++)
    {
        for (int x = 0; x < game.getCellWidth(); x++)
        {
            if (!game.isTileEmpty(x, y)) { drawCell(x, y); }
        }
    }

}

void drawActivePiece()
{
    Piece p = game.getPiece();
    auto shape = p.getShape();

    int px = p.getX();
    int py = p.getY();


    for (int y = 0; y < shape.size(); y++)
    {
        for (int x = 0; x < shape[y].size(); x++)
        {
            // std::cout << shape[y][x];
            if (shape[y][x] > 0) { drawCell(px+x, py+y); }
        }
    }
}

// window and display functions
void windowSize(int w, int h)
{
    glViewport(0, 0, w, h);
    glLoadIdentity();
    gluOrtho2D(0, w, h, 0);
}

void resize(const int w, const int h)
{
    windowSize(w, h);
    game.calculateCellSize(w, h);
}

// display function
void display()
{
    //get width and height
    const int width = glutGet(GLUT_WINDOW_WIDTH);
    const int height = glutGet(GLUT_WINDOW_HEIGHT);

    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    windowSize(width, height);

    // drawing
    drawBackground();
    drawPile();
    drawActivePiece();

    game.update(keys);

    glutSwapBuffers();
}

// framerate limiter/enforcer
void glutTimer(int) {
    glutTimerFunc(1000.0f/FPS, glutTimer, 0);
    glutPostRedisplay();
}


// initialisation
void init(int* pargc, char** argv)
{
    // initialise glut
    glutInit(pargc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);
    glutCreateWindow("Tetris");
    glutInitWindowSize(WINDOW_WIDTH, WINDOW_HEIGHT);

    // set glut functions
    glutDisplayFunc(display);
    glutKeyboardUpFunc(keyboardNormalUp);
    glutSpecialUpFunc(keyboardSpecialUp);
    glutKeyboardFunc(keyboardNormalDown);
    glutSpecialFunc(keyboardSpecialDown);
    glutReshapeFunc(resize);

    // start framerate limiter
    glutTimer(0);

    std::printf("Initialised GLUT\n");

    // get and print rendering device
    const char * gl_renderer = (const char *) glGetString(GL_RENDERER);
    std::printf("Using device: %s\n", gl_renderer);

    // calculate tile size
    game.calculateCellSize(WINDOW_WIDTH, WINDOW_HEIGHT);

}

// main
int main(int argc, char** argv)
{
    init(&argc, argv);

    // start glut
    glutMainLoop();
}