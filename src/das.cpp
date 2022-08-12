#include "das.h"

#include <iostream>

void DAS::start()
{
    this->counting = true;
}

void DAS::stop()
{
    this->counter.reset();
    this->counting = false;
}

void DAS::reset()
{
    this->stop();
    this->start();
}

void DAS::update(DASMode mode)
{

    bool oldActive = this->mode != DASMode::NONE;
    bool newActive =       mode != DASMode::NONE;

    if (oldActive && !newActive) { this->stop(); } // active->stopped
    if (!oldActive && newActive) { this->start(); } // stopped->active

    if (oldActive && newActive) // active -> active
    {
        if (mode != this->mode) { this->reset(); } // if direction changed, then reset DAS. otherwise both are same so tick
        else { this->counter.tick(); }
    }

    // finally, update mode
    this->mode = mode;
}

bool DAS::isReady()
{
    unsigned int elapsed = (unsigned int) this->counter.get();

    if (elapsed == 0) { return true; } // move immediately on keypress
    else if (elapsed == this->initialDelay) { return true; } // if initial delay period has elapsed
    else if ((elapsed > this->initialDelay) && ((elapsed - this->initialDelay) % this->delay == 0)) { return true; } // if delay has been satisfied
    else { return false; } // WE ARE NOT READY!!!1!!
}

unsigned int DAS::getFrame()
{
    return (unsigned int) this->counter.get();
}