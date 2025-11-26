#include "utils/screen.h"
#include "dtekv-lib.h"
#include "wall_texture.h"

#define BACKGROUND_COLOR RGB(0, 4, 0)

#define GREY_COLOR RGB(4, 4, 2)

void fill_background() {
    for (int i = 0; i < SCREEN_WIDTH * SCREEN_HEIGHT; i++) {
        FRAMEBUFFER_ADDRESS[i] = BACKGROUND_COLOR;
    }
}

void draw_square(int x, int y, char color) {
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

void draw_texture(int x, int y, const unsigned char* tex) {
    for (int j = 0; j < 16; j++) {
        for (int i = 0; i < 16; i++) {
            int px = x + i;
            int py = y + j;

            if (px >= 0 && px < SCREEN_WIDTH &&
                py >= 0 && py < SCREEN_HEIGHT) {

                FRAMEBUFFER_ADDRESS[py * SCREEN_WIDTH + px] =
                    tex[j * 16 + i];
            }
        }
    }
}

void draw_border(){
    for (int x = 0; x < SCREEN_WIDTH; x += 16){
        draw_square(x, 0, GREY_COLOR);
        draw_square(x, 16, GREY_COLOR);
    }

    for (int x = 0; x < SCREEN_WIDTH; x += 16){
        draw_texture(x, 32, wall_texture);
        draw_texture(x, SCREEN_HEIGHT - 16, wall_texture);
    }

    for (int y = 32; y < SCREEN_HEIGHT; y += 16){
        draw_texture(0, y, wall_texture);
        draw_texture(SCREEN_WIDTH - 16, y, wall_texture);
    }
}

void draw_inner_squares(){
    for(int x = 32; x < (SCREEN_WIDTH / 2); x += 32){
        for(int y = 64; y < SCREEN_HEIGHT - 32; y += 32){
            draw_texture(x, y, wall_texture);
        }
    }

    for(int x = (SCREEN_WIDTH / 2) + 16; x < SCREEN_WIDTH - 32; x += 32){
        for(int y = 64; y < SCREEN_HEIGHT - 32; y += 32){
            draw_texture(x, y, wall_texture);
        }
    }
}
