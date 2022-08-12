#pragma once

#include <string>

// dissappearing message popup thing.
class Message
{
private:
    std::string msg;
    unsigned int life;

public:
    Message() {};
    ~Message() {};

    std::string getText() const { return this->msg; }

    void set(std::string const &msg, unsigned int duration);
    void clear() { this->msg = ""; }

    // misc
    void tick();
};