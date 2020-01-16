#ifndef MCUINIT_H_INCLUDED
#define MCUINIT_H_INCLUDED


#include <libopencm3/stm32/rcc.h>
#include <libopencm3/stm32/gpio.h>
#include <libopencm3/stm32/usart.h>
#include <libopencm3/cm3/nvic.h>
#include <libopencm3/cm3/systick.h>

#include "defs.h"





extern const iopin_t LED1;



void MCU_Init(void);


#endif /* MCUINIT_H_INCLUDED */
