#pragma once

#include "frametimer.h"

enum class DASMode {LEFT, RIGHT, NONE};

class DAS
{
private:
    FrameCounter counter;

    bool counting;

    unsigned int initialDelay = 16; // inital delay for move
    unsigned int delay = 6; // delay after inital

    // whether the request is left or right
    DASMode mode;

public:
    DAS() {};
    ~DAS() {};

    // called when user presses a movement key
    void start();

    // called when user releases a movement key
    void stop();

    // called each frame
    void update(DASMode mode);

    void reset(); // resets counter but keeps counting

    // determines if movement can be excecuted after frame delay.
    bool isReady();

    // temp
    unsigned int getFrame();

    // gets whether active
    bool isActive() const { return this->counting; }
};
