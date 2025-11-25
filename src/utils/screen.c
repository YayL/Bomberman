#include "utils/screen.h"

#define SCREEN_WIDTH 320
#define SCREEN_HEIGHT 240
#define FRAMEBUFFER_ADDRESS ((volatile char*)0x08000000)
#define BACKGROUND_COLOR RGB(3, 7, 4)
#define RGB(r, g, b) (((r & 0b111) << 5) | ((g & 0b111) << 2) | (b & 0b11))
#define PIXEL_TO_WIDTH(x) (x / SCREEN_WIDTH)
#define PIXEL_TO_HEIGHT(y) (y / SCREEN_HEIGHT)


void fill_background() {
    for (int i = 0; i < SCREEN_WIDTH * SCREEN_HEIGHT; i++)
        FRAMEBUFFER_ADDRESS[i] = BACKGROUND_COLOR; 
}
