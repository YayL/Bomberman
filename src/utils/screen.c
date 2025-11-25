#include "utils/screen.h"
#include <sys/_intsup.h>

#define SCREEN_WIDTH 320
#define SCREEN_HEIGHT 240
#define FRAMEBUFFER_ADDRESS ((volatile char*)0x08000000)
#define BACKGROUND_COLOR RGB(0, 7, 0)
#define BORDER_COLOR RGB(7, 7, 3)
#define RGB(r, g, b) (((r & 0b111) << 5) | ((g & 0b111) << 2) | (b & 0b11))
#define PIXEL_TO_WIDTH(x) (x / SCREEN_WIDTH)
#define PIXEL_TO_HEIGHT(y) (y / SCREEN_HEIGHT)
#define SQUARE_SIZE 16


void fill_background() {
    for (int i = 0; i < SCREEN_WIDTH * SCREEN_HEIGHT; i++)
        FRAMEBUFFER_ADDRESS[i] = BACKGROUND_COLOR; 
}

void make_square(int x, int y, char color) {
    for (int j = 0; j < SQUARE_SIZE; j++) {
        for (int i = 0; i < SQUARE_SIZE; i++) {
            int pixel_x = x + i;
            int pixel_y = y + j;
            if (pixel_x >= 0 && pixel_x < SCREEN_WIDTH && pixel_y >= 0 && pixel_y < SCREEN_HEIGHT) {
                FRAMEBUFFER_ADDRESS[pixel_y * SCREEN_WIDTH + pixel_x] = color;
            }
        }
    }
}

void draw_border(){
    for (int x = 0; x < SCREEN_WIDTH; x += 16){
        make_square(x, 32, BORDER_COLOR);
        make_square(x, SCREEN_HEIGHT - 16, BORDER_COLOR);
    }

    for (int y = 32; y < SCREEN_HEIGHT; y += 16){
        make_square(0, y, BORDER_COLOR);
        make_square(SCREEN_WIDTH - 16, y, BORDER_COLOR);
    }
}

void draw_inner_squares(){
    for(int x = 32; x < SCREEN_WIDTH - 32; x += 32){
        for(int y = 64; y < SCREEN_HEIGHT - 32; y += 32){
            make_square(x, y, BORDER_COLOR);
        }
    }
}
