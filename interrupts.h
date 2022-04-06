#pragma once

void disable_interrupts(void);
void enable_interrupts(void);

void config_GIC(void);

void pushbutton_ISR(void);