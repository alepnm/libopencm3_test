#ifndef GPIO_H_INCLUDED
#define GPIO_H_INCLUDED

#include <libopencm3/stm32/rcc.h>
#include <libopencm3/stm32/gpio.h>
#include <libopencm3/cm3/nvic.h>


typedef struct {
    uint32_t    port;
    uint16_t    pin;
}iopin_t;




#endif /* GPIO_H_INCLUDED */
