#include "das.h"

#include <iostream>

void DAS::start()
{
    this->active = true;
}

void DAS::stop()
{
    this->counter = 0;
    this->active = false;
}

void DAS::reset()
{
    this->counter = 0;
    this->active = true;
}

void DAS::update(Keys const &oldKeys, Keys const &newKeys)
{
    bool oldLeft  = (bool)  oldKeys.left;
    bool oldRight = (bool) oldKeys.right;
    bool newLeft  = (bool)  newKeys.left;
    bool newRight = (bool) newKeys.right;

    bool oldDown = (bool) oldKeys.down;
    bool newDown = (bool) newKeys.down;

    // xor as both left and right are redundant
    bool wasActive = (oldLeft ^ oldRight);
    bool isActive  = (newLeft ^ newRight);

    if (oldDown & !newDown) { this->start(); } // down->not
    else if (!oldDown & newDown) { this->stop(); } // not->down
    else if (!wasActive & isActive) { this->start(); } // stopped->active
    else if (wasActive & !isActive) { this->stop(); } // active->stopped
    else if (wasActive & isActive) // active -> active
    {
        if (oldLeft && newRight || oldRight && newLeft) { this->reset(); } // if direction changed, then reset DAS. otherwise both are same so tick
        else { this->counter++; }
    }
}

bool DAS::isReady()
{
    if (this->counter == 0) { return true; } // move immediately on keypress
    else if (this->counter == this->initialDelay) { return true; } // if initial delay period has elapsed
    else if ((this->counter > this->initialDelay) && ((this->counter - this->initialDelay) % this->delay == 0)) { return true; } // if delay has been satisfied
    else { return false; } // WE ARE NOT READY!!!1!!
}
