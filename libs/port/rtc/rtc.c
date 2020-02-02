
#include "globals.h"
#include "rtc.h"


#define T100MS  0xFFFFFF9B
#define T1000MS 0xFFFFFC17


/*   */
void rtc_init(void)
{
    rtc_awake_from_off(RCC_LSE);

    //rtc_set_prescale_val(16384);      // 500 ms
    //rtc_set_counter_val(0xFFFFFFF0);

    rtc_set_prescale_val(33);           // ~1 ms
    rtc_set_counter_val(T1000MS);       // 1 sec

	nvic_disable_irq(NVIC_RTC_IRQ);
	nvic_set_priority(NVIC_RTC_IRQ, 1);

    rtc_clear_flag(RTC_OW);
    rtc_clear_flag(RTC_ALR);
    rtc_clear_flag(RTC_SEC);

    rtc_interrupt_disable(RTC_ALR);
	rtc_interrupt_disable(RTC_SEC);
	rtc_interrupt_disable(RTC_OW);
}


/*  */
void rtc_process(void)
{
    if(rtc_check_flag(RTC_SEC)){

        rtc_clear_flag(RTC_SEC);

        //gpio_toggle(GPIOC, GPIO13);
    }

    if(rtc_check_flag(RTC_OW)){

        rtc_clear_flag(RTC_OW);

        gpio_toggle(GPIOC, GPIO13);

        rtc_set_counter_val(T100MS);
    }
}


/*
 IRQ RTC_SEC aktyvuojamas, kai preskaleris pasiekia 0 (skaiciuoja COUNT DOWN)
 IRQ RTC_OW aktyvuojamas, kai counter'is pasiekia 0 (skaiciuoja COUNT UP)
*/
void rtc_isr_handler(void)
{
    if(rtc_check_flag(RTC_OW)){

        rtc_clear_flag(RTC_OW);
    }


    if(rtc_check_flag(RTC_SEC)){

        rtc_clear_flag(RTC_SEC);

        //gpio_toggle(GPIOC, GPIO13);
    }
}

/*  */
int bkp_read_register(uint32_t reg, uint16_t* data){

    *data = *(volatile uint32_t*)reg;

    return 0;
}

int bkp_write_register(uint32_t reg, uint16_t data){

    pwr_disable_backup_domain_write_protect();

    *(volatile uint32_t*)reg = data;

    pwr_enable_backup_domain_write_protect();

    return 0;
}




