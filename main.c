#define LED_BASE 			0xFF200000
#define KEY_BASE         	0xFF200050
#define MPCORE_PRIV_TIMER	0xFFFEC600 // 200 MHz Clock -> 2 million cycles every 0.01s
#define HEX3_HEX0_BASE      0xFF200020
#define HEX4_HEX5_BASE	    0xFF200030
#define ADC_BASE 			0xFF204000

#define INIT_LOAD 200000;
#define VOLUME_THRESHOLD 1638
#define LOUD_TIME_THRESHOLD 5000

typedef struct _a9_timer {
    int load;
    int count;
    int control;
    int status;
} a9_timer;

void display_time(unsigned int time);

int main() {
    // Peripheral pointers
    volatile unsigned int* const led_ptr = (unsigned int*)LED_BASE;
    volatile unsigned int* const btn_ptr = (unsigned int*)(KEY_BASE);
    volatile a9_timer* const timer = (a9_timer*)MPCORE_PRIV_TIMER;
    volatile unsigned int* const adc_ptr = (unsigned int*)ADC_BASE;

    // Timer initialization
    timer->status = 1;
    timer->control = 0b0011;
    timer->load = INIT_LOAD;

    // ADC initialization
    adc_ptr[1] = 1;
    int adc_bit_mask = 1 << 16;

    // Variable initializaiton
    unsigned int loud_time = 0;
    unsigned int volume = 0;

    // Main control loop
    while (1) {
        volume = adc_ptr[0];
        if (volume & adc_bit_mask) {
            if ((volume - adc_bit_mask) < VOLUME_THRESHOLD) {
                loud_time = 0;
            }
        }

        if (loud_time > LOUD_TIME_THRESHOLD) {
            *led_ptr = 1;
        }
        else {
            *led_ptr = 0;
        }

        while (timer->status == 0);
        timer->status = 1;
        loud_time++;

        if (*btn_ptr & 0b0001) {
            timer->control = 0b0011;
        }
        else if (*btn_ptr & 0b0010) {
            timer->control = 0b0000;
        }
        else if (*btn_ptr & 0b0100) {
            loud_time = 0;
        }

        display_time(loud_time);
    }
}

void display_time(unsigned int time) {
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

    const unsigned int d7 = binary_lookup[(time / 10000000) % 10];
    const unsigned int d6 = binary_lookup[(time / 1000000) % 10];
    const unsigned int d5 = binary_lookup[(time / 100000) % 10];
    const unsigned int d4 = binary_lookup[(time / 10000) % 10];
    const unsigned int d3 = binary_lookup[(time / 1000) % 10];
    const unsigned int d2 = binary_lookup[(time / 100) % 10];
    const unsigned int d1 = binary_lookup[(time / 10) % 10];
    const unsigned int d0 = binary_lookup[time % 10];

    volatile unsigned int* const hex_ptr0 = (unsigned int*)(HEX3_HEX0_BASE);
    volatile unsigned int* const hex_ptr4 = (unsigned int*)(HEX4_HEX5_BASE);


    *hex_ptr4 = ((d7 << 24) |
        (d6 << 16) |
        (d5 << 8) |
        d4);

    *hex_ptr0 = ((d3 << 24) |
        (d2 << 16) |
        (d1 << 8) |
        d0);
}