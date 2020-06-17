#ifndef TIM_H_INCLUDED
#define TIM_H_INCLUDED

//#include <libopencm3/cm3/cortex.h>
#include <libopencm3/stm32/rcc.h>
#include <libopencm3/cm3/nvic.h>
#include <libopencm3/stm32/timer.h>
#include <libopencm3/stm32/gpio.h>


extern volatile uint32_t systick;

void    tim2_init(void);
void    tim3_init(void);



/*  */
__attribute__((always_inline))
static inline void delay_us(uint32_t delay){

//  CM_ATOMIC_BLOCK() {
    timer_set_counter(TIM3, 0);
    timer_set_period(TIM3, delay-1);

    timer_clear_flag(TIM3, TIM_SR_UIF);

    while(!timer_get_flag(TIM3, TIM_SR_UIF));
//  }
}

#endif /* TIM_H_INCLUDED */
