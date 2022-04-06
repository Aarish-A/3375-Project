#define KEY_BASE         	0xFF200050
#define MPCORE_PRIV_TIMER	0xFFFEC600 // 200 MHz Clock -> 2 million cycles every 0.01s
#define HEX3_HEX0_BASE      0xFF200020
#define HEX4_HEX5_BASE	    0xFF200030
#define KEY_BASE 			0xFF200050
#define SW_BASE				0xFF200040

#define INIT_LOAD 2000000;

typedef struct _a9_timer {
	int load;
	int count;
	int control;
	int status;
} a9_timer;

void display_time(unsigned int time);

int main() {
	unsigned int cur_time;
	unsigned int lap_time;
	unsigned char running = 0;

	volatile a9_timer* const timer = (a9_timer*)MPCORE_PRIV_TIMER;
	timer->load = INIT_LOAD;

	unsigned int cur_count = timer->count;
	unsigned int last_count = timer->count;

	volatile unsigned int* const btn_ptr = (unsigned int*)(KEY_BASE);
	volatile unsigned int* const sw_ptr = (unsigned int*)SW_BASE;

	while (1) {
		cur_count = timer->count;

		if (cur_count > last_count) {
			cur_time++;
		}

		if (*btn_ptr & 0b0001) {
			timer->control = 0b0011;
		}
		else if (*btn_ptr & 0b0010) {
			timer->control = 0b0000;
		}
		else if (*btn_ptr & 0b0100) {
			lap_time = cur_time;
		}
		else if (*btn_ptr & 0b1000) {
			cur_time = 0;
			lap_time = 0;
		}

		if (*sw_ptr == 0) {
			display_time(cur_time);
		}
		else if (*sw_ptr == 1) {
			display_time(lap_time);
		}

		display_time(cur_time);
		last_count = cur_count;
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

	const unsigned int m = time / 6000;
	const unsigned int s = (time - m * 6000) / 100;
	const unsigned int h = time % 100;

	const unsigned int m1 = binary_lookup[m / 10];
	const unsigned int m2 = binary_lookup[m % 10];
	const unsigned int s1 = binary_lookup[s / 10];
	const unsigned int s2 = binary_lookup[s % 10];
	const unsigned int h1 = binary_lookup[h / 10];
	const unsigned int h2 = binary_lookup[h % 10];

	volatile unsigned int* const hex_ptr0 = (unsigned int*)(HEX3_HEX0_BASE);
	volatile unsigned int* const hex_ptr4 = (unsigned int*)(HEX4_HEX5_BASE);

	*hex_ptr4 = ((m1 << 8) |
		m2);

	*hex_ptr0 = ((s1 << 24) |
		(s2 << 16) |
		(h1 << 8) |
		h2);
}



