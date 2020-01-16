#ifndef SYSTICK_H_INCLUDED
#define SYSTICK_H_INCLUDED

#include <libopencm3/cm3/nvic.h>
#include <libopencm3/cm3/systick.h>

extern uint32_t timestamp;


void systick_setup(void);

#endif /* SYSTICK_H_INCLUDED */
