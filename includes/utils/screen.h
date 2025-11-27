#pragma once
#include <stdint.h>
#include <sys/_intsup.h>

#define SCREEN_WIDTH 320
#define SCREEN_HEIGHT 240
#define BLOCK_SIZE 16

#define FRAMEBUFFER_ADDRESS ((volatile char*)0x08000000)

// World Position is the coordinate grid
#define WORLD_X_TO_SCREEN(X) (X * BLOCK_SIZE)
#define WORLD_Y_TO_SCREEN(Y) (Y * BLOCK_SIZE)

// Screen Position is the pixel grid
#define SCREEN_X_TO_WORLD(X) (X / BLOCK_SIZE)
#define SCREEN_Y_TO_WORLD(Y) (Y / BLOCK_SIZE)

// Pixel position is the actual pixel index
#define PIXEL_TO_WORLD_X(P) (P % SCREEN_WIDTH)
#define PIXEL_TO_WORLD_Y(P) (P / SCREEN_WIDTH)

#define WORLD_TO_PIXEL(X, Y) (Y * BLOCK_SIZE * SCREEN_WIDTH + X * BLOCK_SIZE)
#define SCREEN_TO_PIXEL(X, Y) (Y * SCREEN_WIDTH + X)

#define RGB(R, G, B) (((R & 0b111) << 5) | ((G & 0b111) << 2) | (B & 0b11))

void fill_background();
void draw_border();
void draw_inner_squares();
void draw_char(int x, int y, char c, char color);
void draw_word(int x, int y, const char* str, char color);
