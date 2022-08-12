#pragma once

#include <GL/freeglut.h>
#include "main.h"

class FrameCounter
{
private:
    unsigned long long counter;
public:
    FrameCounter() {};
    ~FrameCounter() {};

    unsigned long long get() const { return this->counter; }
    void reset() { this->counter = 0; }
    void tick() { this->counter++; }
};

class FrameTimer
{
private:
    int t1;
    int t2;
    float deltaTime;
    float fps;

    void update();

public:
    FrameTimer() {};
    void tick();

    // gets the elapsed time in seconds since the last frame and the current one
    float getDeltaTime() const { return this->deltaTime; }
    float getFPS() const { return this->fps; }
};