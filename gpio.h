#pragma once


// Structure to manage GPIO register structure
typedef struct _GPIO {
    unsigned int data;
    unsigned int ctrl;
} GPIO;

// Initialize the given GPIO's control register to the specified bit mask (I/O control)
void init_gpio(GPIO* const gpio_ptr, const unsigned int bit_mask);

// Set the given GPIO's inputs to the specified bit pattern
void set_gpio(GPIO* const gpio_ptr, const unsigned int bit_pattern);


void init_gpio(GPIO* const gpio_ptr, const unsigned int bit_mask) {
    // Set the control register of the given GPIO to the specified bit mask
    gpio_ptr->ctrl = bit_mask;
}

void set_gpio(GPIO* const gpio_ptr, const unsigned int bit_pattern) {
    // Set the data register of the given GPIO to specified input bit pattern
    gpio_ptr->data = bit_pattern;
}
