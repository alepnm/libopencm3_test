
#include "globals.h"
#include "gpio.h"


/*   */
void gpio_init(void)
{
    gpio_primary_remap(0, 0);

    rcc_periph_clock_enable(RCC_GPIOA);
    rcc_periph_clock_enable(RCC_GPIOB);
    rcc_periph_clock_enable(RCC_GPIOC);
}



