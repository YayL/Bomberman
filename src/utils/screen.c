#include "utils/screen.h"
#include "dtekv-lib.h"
#include "wall_texture.h"
#include "brick_texture.h"
#include "font8x8_basic.h"

#define GREY_COLOR RGB(4, 4, 2)

#define GRID_WIDTH (SCREEN_WIDTH / BLOCK_SIZE)
#define GRID_HEIGHT (SCREEN_HEIGHT / BLOCK_SIZE)
#define TILE_EMPTY 0
#define TILE_WALL 1
#define TILE_BRICK 2

int world_grid[GRID_WIDTH][GRID_HEIGHT];

static uint32_t rng_state = 0x12345678;

uint32_t fast_rand() {
    rng_state ^= rng_state << 13;
    rng_state ^= rng_state >> 17;
    rng_state ^= rng_state << 5;
    return rng_state;
}

int rand_range(int min, int max) {
    return (fast_rand() % (max - min + 1)) + min;
}

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

void draw_text(int x, int y, const char* str, char color) {
    int current_x = x;
    while(*str){
        draw_char(current_x, y, *str, color);
        current_x += 8;
        str++;
    }
}

void mark_border() {
    for (int x = 0; x < SCREEN_WIDTH; x += 16){
        world_grid[x / 16][2] = TILE_WALL;
        world_grid[x / 16][GRID_HEIGHT - 1] = TILE_WALL;
    }

    for (int y = 32; y < SCREEN_HEIGHT; y += 16){
        world_grid[0][y / 16] = TILE_WALL;
        world_grid[GRID_WIDTH - 1][y / 16] = TILE_WALL;
    }
}

void mark_inner_squares() {
    for(int x = 32; x < (SCREEN_WIDTH / 2); x += 32){
        for(int y = 64; y < SCREEN_HEIGHT - 32; y += 32){
            world_grid[x / 16][y / 16] = TILE_WALL;
        }
    }

    for(int x = (SCREEN_WIDTH / 2) + 16; x < SCREEN_WIDTH - 32; x += 32){
        for(int y = 64; y < SCREEN_HEIGHT - 32; y += 32){
            world_grid[x / 16][y / 16] = TILE_WALL;
        }
    }
}

void mark_safe_zone() {
    world_grid[1][3] = TILE_EMPTY;
    world_grid[1][4] = TILE_EMPTY;
    world_grid[2][3] = TILE_EMPTY;
}

void generate_bricks(float spawn_chance) {
    for(int y = 2; y < GRID_HEIGHT - 1; y++) {
        for(int x = 1; x < GRID_WIDTH - 1; x++) {

            if (world_grid[x][y] != TILE_EMPTY) continue;

            if (rand_range(0, 99) < spawn_chance * 100) {
                world_grid[x][y] = TILE_BRICK;
            }
        }
    }
}

void draw_world() {
    mark_border();
    mark_inner_squares();
    mark_safe_zone();

    //draw header
    for (int x = 0; x < SCREEN_WIDTH; x += 16){
        draw_square(x, 0, GREY_COLOR);
        draw_square(x, 16, GREY_COLOR);
    }

    for (int y = 0; y < GRID_HEIGHT; y++) {
    for (int x = 0; x < GRID_WIDTH; x++) {

        int sx = x * 16;
        int sy = y * 16;

        switch (world_grid[x][y]) {
            case TILE_WALL:
                draw_texture(sx, sy, wall_texture);
                break;

            case TILE_BRICK:
                draw_texture(sx, sy, brick_texture);
                break;

            default:
                break;
            }
        }
    }
}