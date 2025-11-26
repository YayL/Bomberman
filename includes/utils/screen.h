#pragma once
#include <stdint.h>
#include <sys/_intsup.h>

#define SCREEN_WIDTH 320
#define SCREEN_HEIGHT 240
#define BLOCK_SIZE 16

#define VGA_DMA_CONTROLLER_ADDRESS ((volatile uint32_t *) 0x04000100)
#define FRAMEBUFFER_1_ADDRESS ((volatile char*)0x08000000)
#define FRAMEBUFFER_2_ADDRESS ((volatile char*)0x08000000 + SCREEN_WIDTH * SCREEN_HEIGHT)

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

// World grid definitions
#define GRID_WIDTH (SCREEN_WIDTH / BLOCK_SIZE)
#define GRID_HEIGHT (SCREEN_HEIGHT / BLOCK_SIZE)
#define TILE_EMPTY 0
#define TILE_WALL 1
#define TILE_BRICK 2
#define TILE_BOMB 3
extern int world_grid[GRID_WIDTH][GRID_HEIGHT];

//Functions
void fill_background(char color);
void generate_bricks(float spawn_chance);
void draw_world();
void mark_bomb(int x, int y);
void mark_world();
#define BLACK RGB(0, 0, 0)
#define LIGHT_GREY RGB(7, 7, 7)
#define GREY RGB(4, 4, 2)
#define WHITE RGB(7, 7, 3)
#define YELLOW RGB(7, 5, 0)
#define BACKGROUND_GREEN RGB(0, 4, 0)

void screen_init();
void screen_blit();
volatile char * screen_get_framebuffer_addr();

void fill_background(uint8_t color);
void draw_rectangle(uint32_t x_start, uint32_t y_start, uint32_t width, uint32_t height, uint8_t color);
void draw_texture(uint32_t x_start, uint32_t y_start, const uint8_t * tex);
void draw_char(uint32_t x, uint32_t y, char c, uint8_t color);
void draw_word(uint32_t x, uint32_t y, const char * str, uint8_t color);
