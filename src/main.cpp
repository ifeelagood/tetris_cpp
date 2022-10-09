#include "tetris.h"
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

#include <iostream>
#include <thread>
#include <chrono>

#include "common.h"
#include "bag.h"
#include "board.h"
#include "levels.h"
#include "renderer.h"
#include "keys.h"
#include "keybinds.h"

#define TARGET_FPS 60.0

// GLFW CALLBACKS
static void windowSizeCallback(GLFWwindow *window, int w, int h);
static void keyboardCallback(GLFWwindow *window, int key, int scancode, int action, int mods);

// OPENGL FUNCTIONS
void updateProjection(GLFWwindow *window, int &width, int &height);

// GAME FUNCTIONS
void resetGame();
void spawnNewPiece(); // used at the start of the game as well as when a piece is locked
void levelUp();

void DAS(); // delay auto shift
void doGravityDrop(); // moves the piece down via gravity
void doSoftDrop(); // moves the piece down via softdrop
void doRotation(); 


// GLOBALS
KeyManager keyManager; // stores key states and acts as an interface to get pressed,down,etc
Renderer renderer; // handles rendering and resource manangement
BoardManager boardManager; // handles board logic, including piece movement, collision, etc
Bag bag; // bag of tetrominos

unsigned long long frame; // Frame counter
unsigned char das_l; // left DAS counter
unsigned char das_r; // right DAS counter

unsigned int topScore = 10000; // NES tetris default
unsigned int score;
unsigned int level;
unsigned int lines;

bool gameOver;

// main
int main()
{
    GLFWwindow *window;

    if (!glfwInit())
    {
        std::cerr << "Failed to initialize GLFW\n";
        return -1;
    }

    window = glfwCreateWindow(640, 480, "Tetris", NULL, NULL);
    if (!window)
    {
        std::cerr << "Failed to create GLFW window\n";
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);

    glfwSetKeyCallback(window, keyboardCallback);
    glfwSetWindowSizeCallback(window, windowSizeCallback);

    // initialize glew
    if (!gladLoadGL(glfwGetProcAddress))
    {
        std::cerr << "Failed to initialize OpenGL context\n";
        glfwTerminate();
        return -1;
    }

    double targetFrameTime = 1.0 / TARGET_FPS;

    // initialise game
    resetGame();

    // main game loop
    while (!glfwWindowShouldClose(window))
    {
        double startTime = glfwGetTime(); // needed for framerate limiting

        // update game logic
        DAS();
        doGravityDrop();
        doSoftDrop();
        doRotation();

        levelUp();

        // prepare next frame as a texture
        renderer.clearBuffer();
        renderer.blitPlayfield(boardManager.getPile(), boardManager.getActivePiece(), level);
        renderer.blitUI(bag.peekNext(), level, score, 10000, lines, gameOver);
        renderer.generateTexture();

        // update projection 
        int width, height;
        updateProjection(window, width, height);

        // draw texture     
        renderer.draw(width, height);

        // swap buffers
        glfwSwapBuffers(window);

        // poll events and update keymanager
        keyManager.update(); // move newKeys to oldKeys
        glfwPollEvents(); // then poll for new events

        // limit framerate
        double endTime = glfwGetTime();
        double frameTime = endTime - startTime;
        if (frameTime < targetFrameTime)
        {
            double sleepTime = targetFrameTime - frameTime;
            std::this_thread::sleep_for(std::chrono::duration<double>(sleepTime));
        }

        // Do game over check. After framerate limiter.
        if (gameOver)
        {
            if (score > topScore)
            {
                topScore = score;
            }
            std::this_thread::sleep_for(std::chrono::duration<double>(5.0));
            resetGame();
        }

        frame++;
    }
}


// glfw callbacks
static void windowSizeCallback(GLFWwindow *window, int w, int h)
{
    glViewport(0, 0, w, h);
}

static void keyboardCallback(GLFWwindow *window, int key, int scancode, int action, int mods)
{
    if (action == GLFW_PRESS)
    {
        if (key == GLFW_KEY_ESCAPE)
        {
            glfwSetWindowShouldClose(window, GLFW_TRUE);
        }
        if (key >= 0 && key < 1024)
        {
            keyManager.setKey(key, true);
        }
    }

    else if (action == GLFW_RELEASE)
    {
        if (key >= 0 && key < 1024)
        {
            keyManager.setKey(key, false);
        }
    }
}

// opengl function
void updateProjection(GLFWwindow *window, int &width, int &height)
{
    glfwGetFramebufferSize(window, &width, &height);
    
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0, width, height, 0, -1, 1);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

// game functions
void resetGame()
{
    frame = 0;
    das_l = 0;
    das_r = 0;
    score = 0;
    level = 0;
    lines = 0;
    gameOver = false;

    boardManager.reset();
    bag.reset();
    spawnNewPiece();
}

void spawnNewPiece()
{
    Tetromino newMinoShape = bag.getNext();
    Piece newPiece(newMinoShape);

    boardManager.setActivePiece(newPiece);
}

void levelUp()
{
    if (lines >= (level + 1) * 10)
    {
        level++;
    }
}

void DAS()
{
    if (! (keyManager.isKeyDown(KEY_MOVE_LEFT) && keyManager.isKeyDown(KEY_MOVE_RIGHT)) )
    {
        // reset counter on first key press
        if (keyManager.isKeyPressed(KEY_MOVE_LEFT))
        {
            das_l = 0;
            boardManager.moveActiveLeft();
        }
        else if (keyManager.isKeyDown(KEY_MOVE_LEFT))
        {
            das_l++;
            if (das_l == DAS_INITIAL_DELAY) // if inital delay is met, then set counter back.
            {
                das_l = DAS_INITIAL_DELAY - DAS_REPEAT_DELAY;
                boardManager.moveActiveLeft();
            }
        }

        // reset counter on first key press
        if (keyManager.isKeyPressed(KEY_MOVE_RIGHT))
        {
            das_r = 0;
            boardManager.moveActiveRight();
        }
        else if (keyManager.isKeyDown(KEY_MOVE_RIGHT))
        {
            das_r++;
            if (das_r == DAS_INITIAL_DELAY) // if inital delay is met, then set counter back.
            {
                das_r = DAS_INITIAL_DELAY - DAS_REPEAT_DELAY;
                boardManager.moveActiveRight();
            }
        }
    }
}

void doGravityDrop()
{
    if (frame % LevelGravity[level] == 0)
    {
        // if piece is landed, check for top out. otherwise, lock it and spawn a new piece
        if (boardManager.isLanded())
        {
            if (boardManager.lockPiece()) // if topped out
            {
                gameOver = true;
            }
            else 
            {
                unsigned char linesCleared = boardManager.clearLines();
                if (linesCleared > 0)
                {
                    score += LineScore[linesCleared] * (level + 1);
                    lines += linesCleared;
                }
                spawnNewPiece();
            }
        }
        else // piece hasnt landed, move down
        {
            boardManager.moveActiveDown();
        }
    }
}

void doSoftDrop()
{
    if (keyManager.isKeyDown(KEY_MOVE_DOWN))
    {
        if (!boardManager.isLanded())
        {
            if (frame % 2 == 0)
            {
                boardManager.moveActiveDown();
                score++;
            }
        }
    }
}

void doRotation()
{
    if (keyManager.isKeyPressed(KEY_ROTATE_LEFT))
    {
        boardManager.rotateActiveLeft();
    }
    if (keyManager.isKeyPressed(KEY_ROTATE_RIGHT))
    {
        boardManager.rotateActiveRight();
    }
}