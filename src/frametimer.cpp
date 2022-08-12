#include "frametimer.h"
#include <unistd.h>


void FrameTimer::tick()
{
    this->t2 = this->t1;
    this->t1 = glutGet(GLUT_ELAPSED_TIME);

    this->deltaTime = (this->t1 - this->t2) / 1000.0f;
    this->fps = 1.0f / this->deltaTime;
}


