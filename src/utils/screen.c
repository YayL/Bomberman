/*
*   Contributors: Axel & Zimon
*
*   Axel:
*        - Initial implementation
*        - Screen drawing functions
*
*    Zimon:
*        - VGA DMA and screen buffer handling
*        - Optimized draw calls for higher FPS
*/


#include "utils/screen.h"
#include "dtekv-lib.h"
#include "assets/font.h"
#include "utils/mem.h"

// The current drawable frame buffer
static volatile char * FRAMEBUFFER_ADDRESS = FRAMEBUFFER_2_ADDRESS;

volatile struct vga_dma_controller {
    volatile char * buffer;
    volatile char * backbuffer;

    volatile struct vga_dma_controller_resolution {
        uint16_t x;
        uint16_t y;
    } resolution;

    volatile struct vga_dma_controller_status {
        uint32_t swap_status : 1;
        uint32_t addr_mode : 1;
        uint32_t _ : 2;
        uint32_t bytes_of_color : 4;
        uint8_t x_coord_addr_with;
        uint8_t y_coord_addr_with;
    } status;
} * vga_dma_controller = (volatile struct vga_dma_controller *) VGA_DMA_CONTROLLER_ADDRESS;

void screen_init() {
    fill_background(BLACK);
    screen_blit();
}

void screen_blit() {
    vga_dma_controller->backbuffer = FRAMEBUFFER_ADDRESS; // Draw framebuffer
    vga_dma_controller->buffer = 0; // request swap

    FRAMEBUFFER_ADDRESS = (FRAMEBUFFER_ADDRESS == FRAMEBUFFER_1_ADDRESS)
                            ? FRAMEBUFFER_2_ADDRESS
                            : FRAMEBUFFER_1_ADDRESS;

    // Wait for buffer swap so we can start drawing to the previously displayed buffer
    while (vga_dma_controller->status.swap_status);

    if (FRAMEBUFFER_ADDRESS == vga_dma_controller->buffer) {
        puts("NOO!\n");
        exit();
    }
}

volatile char * screen_get_framebuffer_addr() {
    return FRAMEBUFFER_ADDRESS;
}

void fill_background(uint8_t color) {
    const uint32_t pattern = (color << 24) | (color << 16) | (color << 8) | color;
    const uint32_t num_words = SCREEN_WIDTH * SCREEN_HEIGHT;
    memset(FRAMEBUFFER_ADDRESS, pattern, num_words);
}

void draw_rectangle(uint32_t x_start, uint32_t y_start, uint32_t width, uint32_t height, uint8_t color) {
    const uint32_t pattern = (color << 24) | (color << 16) | (color << 8) | (color);

    for (int y = 0; y < height; y++) {
        const uint32_t fb_addr_offset = (y_start + y) * SCREEN_WIDTH + x_start;
        memset(&FRAMEBUFFER_ADDRESS[fb_addr_offset], pattern, width);
    }
}

void draw_texture(uint32_t x_start, uint32_t y_start, const uint8_t * tex) {
    if (SCREEN_WIDTH < x_start + BLOCK_SIZE) {
        puts("Texture draw OOB: X");
        exit();
    } else if (SCREEN_HEIGHT < y_start + BLOCK_SIZE) {
        puts("Texture draw OOB: Y");
        exit();
    }

    const uint32_t tex_height = BLOCK_SIZE;
    const uint32_t tex_width = BLOCK_SIZE;

    for (uint32_t y = 0; y < tex_height; y++) {
        const uint32_t fb_addr_offset = (y_start + y) * SCREEN_WIDTH + x_start;
        memcpy(&FRAMEBUFFER_ADDRESS[fb_addr_offset], &tex[y * tex_width], tex_width);
    }
}

void draw_pixel(uint32_t x, uint32_t y, uint8_t color) {
    if (x < SCREEN_WIDTH && y < SCREEN_HEIGHT) {
        FRAMEBUFFER_ADDRESS[y * SCREEN_WIDTH + x] = color;
    }
}

void draw_char(uint32_t x, uint32_t y, char c, uint8_t color) {
    uint8_t * bitmap = (uint8_t *) FONT[(uint8_t) c];

    for (int i = 0; i < 8; i++) {
        uint8_t bits = bitmap[i];
        for (int j = 0; j < 8; j++) {
            if (bits & (1 << j)) {
                draw_pixel(x + j, y + i, color);
            }
        }
    }
}

void draw_text(uint32_t x, uint32_t y, const char * str, uint8_t color) {
    uint32_t current_x = x;

    while (*str) {
        draw_char(current_x, y, *str, color);
        current_x += 8;
        str++;
    }
}
