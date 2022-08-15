#pragma once

#define WINDOW_WIDTH 1000
#define WINDOW_HEIGHT 1000
#define BOARD_WIDTH 10
#define BOARD_HEIGHT 20

#define FPS 60.0

// ##### NOTES #####
// https://tetris.fandom.com/wiki/Tetris_(NES,_Nintendo)

// DAS initial delay is 16 frames, and then every 6 frames.

// from https://harddrop.com/wiki/Tetris_(NES,_Nintendo):

// Despite the title's popularity, its DAS mechanics were not well-understood
// until player Kitaru analyzed its internal memory values. The game keeps a
// hidden count of how many frames the left or right button has been held down
// for. This is called the "DAS counter." After holding down left or right for
// 16 frames, the piece moves left or right by one column, and the DAS counter
// resets. Only, it doesn't actually reset all the way back to 0. It instead
// resets it to 10. After 6 more frames, it will reach 16, the piece will move
// another column, and the counter will reset back to 10. After the left or
// right button is released, and left or right is pressed again, the counter
// will reset all the way back to zero. In effect, pressing and holding down the
// left button will first move the piece 1 column, then after 16 frames it moves
// another column, then it will move a column every 6 frames.


// https://github.com/Elanif/NESTRIS/blob/4b236287f5d6791c404aea4b0417e2b36d94ac75/nestris%20rules.txt