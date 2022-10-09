#include "keys.h"

#include <string.h>
#include <iostream> // debugging

void KeyManager::update()
{
    // copy keys
    memcpy(oldKeys, newKeys, sizeof(Keys));
}

bool KeyManager::isKeyDown(int key)
{
    return newKeys[key];
}

bool KeyManager::isKeyUp(int key)
{
    return !newKeys[key];
}

bool KeyManager::isKeyPressed(int key)
{
    return newKeys[key] && !oldKeys[key];
}

bool KeyManager::isKeyReleased(int key)
{
    return !newKeys[key] && oldKeys[key];
}

void KeyManager::setKey(int key, bool state)
{
    newKeys[key] = state;
}