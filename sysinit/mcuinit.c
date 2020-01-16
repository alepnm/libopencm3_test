
#include "mcuinit.h"
#include "usart.h"


const iopin_t LED1 = { GPIOC, GPIO13 };




static void clock_setup(void)
{
    rcc_clock_setup_in_hse_8mhz_out_72mhz();

    rcc_periph_clock_enable(RCC_GPIOA);
    rcc_periph_clock_enable(RCC_GPIOB);
    rcc_periph_clock_enable(RCC_GPIOC);

    rcc_periph_clock_enable(RCC_USART1);
    rcc_periph_clock_enable(RCC_USART2);
    rcc_periph_clock_enable(RCC_USART3);
}


static void gpio_setup(void)
{
    /* Enable GPIO clock for leds. */
    rcc_periph_clock_enable(RCC_GPIOC);

    /* Enable led as output */
    gpio_set_mode(LED1.port, GPIO_MODE_OUTPUT_50_MHZ, GPIO_CNF_OUTPUT_PUSHPULL, LED1.pin);

    gpio_set(LED1.port, LED1.pin);
}


void MCU_Init(void)
{
    clock_setup();
    gpio_setup();
    //usart1_setup();
    //systick_setup();
}
