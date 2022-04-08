#pragma once


// Bit mask to determine whether the status bit on the ADC is set
const unsigned int bit_mask = 1 << 15;

// Initialize the ADC channels to auto-update
void init_adc(unsigned int* const adc_ptr);

// Return the ADC reading from a given channel 
// ** return value might be garbage, needs to be verified **
unsigned int get_reading(unsigned int* const adc_ptr, unsigned char channel);

// Verify whether a given ADC reading is valid by checking the status bit
// Returns 0 if invalid, 1 if valid
unsigned char verify_adc_data(unsigned int data);

// Return a verified ADC reading by discounting the status bit from reading
unsigned int get_verified_adc_data(unsigned int data);


void init_adc(unsigned int* const adc_ptr) {
    // Set channel 2 to auto update all channels
    adc_ptr[1] = 1;
}

unsigned int get_adc_reading(unsigned int* const adc_ptr, const unsigned char channel) {
    // Return an unverifed reading from a given channel on the ADC
    return adc_ptr[channel];
}

unsigned char verify_adc_data(unsigned int data) {
    // Return 1 if status bit is sets, 0 if it is not set
    return data & bit_mask;
}

unsigned int get_verified_adc_data(unsigned int data) {
    // Return the verified data, less the status bit
    return data - bit_mask;
}