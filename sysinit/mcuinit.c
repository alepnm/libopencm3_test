
#include <libopencm3/cm3/cortex.h>

#include "mcuinit.h"
#include "systick.h"
#include "usart.h"
#include "e24lcxx.h"
#include "ds1307.h"
#include "mcp.h"
#include "rtc.h"


const iopin_t LED1 = { GPIOC, GPIO13 };

/*  */
static void clock_setup(void)
{
    rcc_clock_setup_in_hse_8mhz_out_72mhz();

    rcc_periph_clock_enable(RCC_GPIOA);
    rcc_periph_clock_enable(RCC_GPIOB);
    rcc_periph_clock_enable(RCC_GPIOC);

    rcc_periph_clock_enable(RCC_USART1);
    rcc_periph_clock_disable(RCC_USART2);
    rcc_periph_clock_disable(RCC_USART3);
}

/*  */
static void gpio_setup(void)
{
    gpio_primary_remap(0, 0);   // no remap

    /* Enable GPIO clock for leds. */
    rcc_periph_clock_enable(RCC_GPIOC);

    /* Enable led as output */
    gpio_set_mode(LED1.port, GPIO_MODE_OUTPUT_50_MHZ, GPIO_CNF_OUTPUT_PUSHPULL, LED1.pin);

    gpio_set(LED1.port, LED1.pin);
}

/*  */
void MCU_Init(void)
{

    //cm_disable_interrupts();

    clock_setup();
    //systick_setup();
    gpio_setup();
    usart_init();
    eep_init(eeprom);
    //rtc_init();
    ds_init(ds1307);
    //mcp_init( mcp );

    //cm_enable_interrupts();
}
