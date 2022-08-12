#include "ui.h"
#include "main.h" // for fps

void Message::set(std::string const &msg, unsigned int duration)
{
    this->life = duration * FPS;
    this->msg = msg;
}

void Message::tick()
{
    if (this->life) { life--; }
    else { this->msg = ""; }
}