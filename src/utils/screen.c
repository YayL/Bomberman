#include "utils/screen.h"
#include "dtekv-lib.h"
#include "wall_texture.h"
#include "font8x8_basic.h"

#define BACKGROUND_COLOR RGB(0, 4, 0)

#define GREY_COLOR RGB(4, 4, 2)

void fill_background(char color) {
    for (int i = 0; i < SCREEN_WIDTH * SCREEN_HEIGHT; i++) {
        FRAMEBUFFER_ADDRESS[i] = color;
    }
}

void draw_pixel(int x, int y, char color) {
    if (x >= 0 && x < SCREEN_WIDTH && y >= 0 && y < SCREEN_HEIGHT) {
        FRAMEBUFFER_ADDRESS[y * SCREEN_WIDTH + x] = color;
    }
}

void draw_square(int x, int y, char color) {
    for (int j = 0; j < BLOCK_SIZE; j++) {
        for (int i = 0; i < BLOCK_SIZE; i++) {
            int pixel_x = x + i;
            int pixel_y = y + j;
            draw_pixel(pixel_x, pixel_y, color);
        }
    }
}

void draw_texture(int x, int y, const unsigned char* tex) {
    for (int j = 0; j < 16; j++) {
        for (int i = 0; i < 16; i++) {
            int pixel_x = x + i;
            int pixel_y = y + j;
            draw_pixel(pixel_x, pixel_y, tex[j * 16 + i]);
        }
    }
}

void draw_char(int x, int y, char c, char color) {
    uint8_t *bitmap = (uint8_t *)font8x8_basic[(uint8_t)c];

    for(int i = 0; i < 8; i++){
        uint8_t bits = bitmap[i];
        for(int j = 0; j < 8; j++){
            if(bits & (1 << j)){
                draw_pixel(x + j, y + i, color);
            }
        }
    }
}

void draw_word(int x, int y, const char* str, char color) {
    int current_x = x;
    while(*str){
        draw_char(current_x, y, *str, color);
        current_x += 8;
        str++;
    }
}

void draw_border() {
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

void draw_inner_squares() {
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
