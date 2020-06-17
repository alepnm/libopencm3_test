#ifndef TIM_H_INCLUDED
#define TIM_H_INCLUDED


#include <libopencm3/stm32/rcc.h>
#include <libopencm3/cm3/nvic.h>
#include <libopencm3/stm32/timer.h>
#include <libopencm3/stm32/gpio.h>


extern volatile uint32_t systick;

void    tim2_init(void);

#endif /* TIM_H_INCLUDED */
