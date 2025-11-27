#include "utils/button.h"

#define  BUTTON_ADDRESS 0x040000d0

volatile struct button {
    uint32_t data;
} *button = (void *) BUTTON_ADDRESS;

uint32_t button_get_state(){
    return (button->data);
}