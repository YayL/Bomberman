#include "utils/screen.h"
#include "dtekv-lib.h"

#define BACKGROUND_COLOR1 RGB(4, 7, 0)
#define BACKGROUND_COLOR2 RGB(4, 7, 2)

#define BORDER_COLOR RGB(7, 7, 3)

void fill_background() {
    char type = 0; 

    for (int pixel = 0; pixel < SCREEN_WIDTH * SCREEN_HEIGHT; pixel++) {
        if (pixel % BLOCK_SIZE == 0 && pixel % (SCREEN_WIDTH * BLOCK_SIZE) != 0) {
            type = !type;
        }

        FRAMEBUFFER_ADDRESS[pixel] = (type ? BACKGROUND_COLOR1 : BACKGROUND_COLOR2);
    }
}

void make_square(int x, int y, char color) {
    for (int j = 0; j < BLOCK_SIZE; j++) {
        for (int i = 0; i < BLOCK_SIZE; i++) {
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
