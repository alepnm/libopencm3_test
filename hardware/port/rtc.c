
#include "globals.h"
#include "rtc.h"


volatile uint32_t timestamp;

volatile uint8_t UpdateDateTimeRequired = false;




/*  inicializuojam RTC */
void rtc_init(void)
{
    rtc_auto_awake(RCC_LSE, 32767);

    nvic_enable_irq(NVIC_RTC_IRQ);
    nvic_set_priority(NVIC_RTC_IRQ, 1);

    rtc_clear_flag(RTC_OW);
    rtc_clear_flag(RTC_ALR);
    rtc_clear_flag(RTC_SEC);

    rtc_interrupt_disable(RTC_ALR);
    rtc_interrupt_disable(RTC_OW);
    rtc_interrupt_enable(RTC_SEC);

    /* backup registrai */
    if(BKP_USER_REGISTER1 != 0xAA55)
    {
        rtc_set_counter_val(0);

        pwr_disable_backup_domain_write_protect();

        /* cia reikia sukonfiguruoti BKP registrus - irasyti defoltinius datetime */

        BKP_USER_REGISTER1 = 0xAA55;
        pwr_enable_backup_domain_write_protect();
    }

    timestamp = rtc_get_counter_val();
}


/*
 IRQ RTC_SEC aktyvuojamas, kai preskaleris pasiekia 0 (skaiciuoja COUNT DOWN)
 IRQ RTC_OW aktyvuojamas, kai counter'is pasiekia 0 (skaiciuoja COUNT UP)
*/
void rtc_isr(void)
{
    if(rtc_check_flag(RTC_SEC))
    {
        rtc_clear_flag(RTC_SEC);

        timestamp = rtc_get_counter_val();

        UpdateDateTimeRequired = true;
    }
}



