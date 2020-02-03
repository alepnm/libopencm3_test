#ifndef SYSTICK_H_INCLUDED
#define SYSTICK_H_INCLUDED

#include <libopencm3/cm3/nvic.h>
#include <libopencm3/cm3/systick.h>


#define systick_isr_handler SysTick_IRQHandler

extern uint32_t systick;


void systick_setup(void);
void systick_handler(void);

#endif /* SYSTICK_H_INCLUDED */
