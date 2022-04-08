#pragma once


// Lookup table to map a given digit (index) to its corresponding
// bit pattern on the 7 segment display
const int binary_lookup[16] = {
    0b00111111,
    0b00000110,
    0b01011011,
    0b01001111,
    0b01100110,
    0b01101101,
    0b01111101,
    0b00000111,
    0b01111111,
    0b01100111,
    0b00000000,
    0b00000000,
    0b00000000,
    0b00000000,
    0b00000000,
    0b00000000
};

// Display the given time in milliseconds on the first 8 7 segment displays
void display_ms(unsigned int ms, unsigned int* const hex_ptr0, unsigned int* const hex_ptr4);


void display_ms(unsigned int ms, unsigned int* const hex_ptr0, unsigned int* const hex_ptr4) {
    // Determine bit patterns of digits 0-7 of the milliseconds reading
    const unsigned int d7 = binary_lookup[(ms / 10000000) % 10];
    const unsigned int d6 = binary_lookup[(ms / 1000000) % 10];
    const unsigned int d5 = binary_lookup[(ms / 100000) % 10];
    const unsigned int d4 = binary_lookup[(ms / 10000) % 10];
    const unsigned int d3 = binary_lookup[(ms / 1000) % 10];
    const unsigned int d2 = binary_lookup[(ms / 100) % 10];
    const unsigned int d1 = binary_lookup[(ms / 10) % 10];
    const unsigned int d0 = binary_lookup[ms % 10];

    // Display digits 4-7 on the last 4 seven segment displays
    *hex_ptr4 = ((d7 << 24) |
        (d6 << 16) |
        (d5 << 8) |
        d4);

    // Display digits 0-3 on the first 4 seven segment displays
    *hex_ptr0 = ((d3 << 24) |
        (d2 << 16) |
        (d1 << 8) |
        d0);
}