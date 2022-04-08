#include "address_map.h"
#include "constants.h"
#include "timer.h"
#include "adc.h"
#include "sevensegdisplay.h"
#include "gpio.h"


int main() {
	// Peripheral pointers
	volatile unsigned int* const led_ptr = (unsigned int*)LED_BASE;
	volatile unsigned int* const btn_ptr = (unsigned int*)KEY_BASE;
	volatile a9_timer* const timer = (a9_timer*)MPCORE_PRIV_TIMER;
	volatile unsigned int* const adc_ptr = (unsigned int*)ADC_BASE;
	volatile unsigned int* const hex_ptr0 = (unsigned int*)HEX3_HEX0_BASE;
	volatile unsigned int* const hex_ptr4 = (unsigned int*)HEX4_HEX5_BASE;
	volatile GPIO* const gpio_ptr = (GPIO*)GPIO_BASE;

	// Peripheral initialization
	init_timer(timer);
	init_adc(adc_ptr);
	init_gpio(gpio_ptr, 0x3FF);

	// Variable initializaiton
	const unsigned char adc_channel = 0; // ADC channel to use for volume reading
	unsigned int loud_time = 0;			 // Keeps track of the time a loud volume is detected (ms)
	unsigned int volume = 0;			 // Keeps track of the volume reading from the ADC

	// Main control loop
	while (1) {
		// Obtain the ADC reading
		volume = get_adc_reading(adc_ptr, adc_channel);

		// If the ADC reading is valid (bit 15 status flag is set)
		if (verify_adc_data(volume)) {
			// Obtain the actual verified volume data
			int verified_volume = get_verified_adc_data(volume);

			// Determine which LED to light up for verifying ADC value (testing only)
			int n = (verified_volume / 409);

			// Set the LED to verify ADC value (testing only)
			set_gpio(gpio_ptr, 1 << n);

			// If the actual volume is less than the threshold, reset loud time
			if (verified_volume < VOLUME_THRESHOLD) {
				loud_time = 0;
			}
		}

		// Wait for the timer to count down 1ms and increment loud timer
		wait_for_timer(timer);
		loud_time++;

		// If the loud time is greater than the threshold (30m usually)
		// turn the led on to simulate the motor being turned on, 
		// otherwise turn the led off
		if (loud_time > LOUD_TIME_THRESHOLD)
			*led_ptr = 1;
		else
			*led_ptr = 0;

		// For testing, reset loud timer when button 2 is pressed
		if (*btn_ptr & 0b0100)
			loud_time = 0;

		// Display the current loud time on the 7 segment displays
		display_ms(loud_time, hex_ptr0, hex_ptr4);
	}
}