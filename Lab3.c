#define LED_BASE 0xFF200000
#define SW_BASE	 0xFF200040
#define ADC_BASE 0xFF204000

	
void main() {
	volatile unsigned int* const led_ptr = (unsigned int*)LED_BASE;
	volatile unsigned int* const sw_ptr = (unsigned int*)SW_BASE;
	volatile unsigned int* const adc_arr = (unsigned int*)ADC_BASE;
	
	adc_arr[0] = 1;
	adc_arr[1] = 1;
	int bit_mask = 1<<16;
	
	while(1) {
		int adc_data = adc_arr[*sw_ptr];
			
		if (adc_arr[*sw_ptr] & bit_mask) {
			*led_ptr = adc_data;
		}
	}
}
	
	