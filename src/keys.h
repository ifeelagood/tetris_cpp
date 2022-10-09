#ifndef KEYS_H
#define KEYS_H

#include "common.h"

typedef bool Keys[1024];

class KeyManager
{
private:
    Keys oldKeys;
    Keys newKeys;

public:
    KeyManager() {};
    ~KeyManager() {};

    void update(); // copies newKeys to oldKeys

    bool isKeyDown(int key); // is key currently down
    bool isKeyUp(int key);   // is key currently up

    bool isKeyPressed(int key); // was key pressed this frame
    bool isKeyReleased(int key); // was key released this frame

    void setKey(int key, bool state);
};

#endif