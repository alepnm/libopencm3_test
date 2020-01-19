
#include <libopencm3/cm3/cortex.h>

#include "rtc.h"
#include "mcuinit.h"


/*   */
void rtc_init(void)
{
    rcc_enable_rtc_clock();

    rtc_awake_from_off(RCC_LSE);

    rtc_set_prescale_val(3276);
    rtc_set_counter_val(0x00);

	nvic_enable_irq(NVIC_RTC_IRQ);
	nvic_set_priority(NVIC_RTC_IRQ, 1);

    cm_disable_interrupts();

    rtc_clear_flag(RTC_OW);
    rtc_clear_flag(RTC_ALR);
    rtc_clear_flag(RTC_SEC);

    rtc_interrupt_disable(RTC_ALR);
	rtc_interrupt_disable(RTC_SEC);
	rtc_interrupt_disable(RTC_OW);

	cm_enable_interrupts();
}


/*  */
void rtc_handler(void)
{
    if(rtc_check_flag(RTC_SEC)){

        rtc_clear_flag(RTC_SEC);

        gpio_toggle(LED1.port, LED1.pin);
    }

}


/*  */
void rtc_isr_handler(void)
{
    if(rtc_check_flag(RTC_OW)){

        rtc_clear_flag(RTC_OW);
    }


    if(rtc_check_flag(RTC_SEC)){

        rtc_clear_flag(RTC_SEC);

        //gpio_toggle(LED1.port, LED1.pin);
    }
}
