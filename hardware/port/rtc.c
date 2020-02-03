
#include "globals.h"
#include "rtc.h"


#define T60SEC  0xFFFFFFC3


struct _dt datetime;

volatile uint32_t rtc_timestamp = 0;
volatile uint8_t UpdateDateTimeRequired = false;

/*   */
void rtc_init(void)
{
    rtc_awake_from_off(RCC_LSE);

    rtc_set_prescale_val(32768);        // 1 sec
    rtc_set_counter_val(T60SEC);        // 60 sec

	nvic_enable_irq(NVIC_RTC_IRQ);
	nvic_set_priority(NVIC_RTC_IRQ, 1);

    rtc_clear_flag(RTC_OW);
    rtc_clear_flag(RTC_ALR);
    rtc_clear_flag(RTC_SEC);

    rtc_interrupt_disable(RTC_ALR);
	rtc_interrupt_enable(RTC_SEC);
	rtc_interrupt_disable(RTC_OW);



	if(BKP_USER_REGISTER1 != 0xAA55){

        pwr_disable_backup_domain_write_protect();


        BKP_USER_REGISTER1 = 0xAA55;
        pwr_enable_backup_domain_write_protect();
	}
}


/*  */
int rtc_datetime_process(void){


    return 0;
}


/*
 IRQ RTC_SEC aktyvuojamas, kai preskaleris pasiekia 0 (skaiciuoja COUNT DOWN)
 IRQ RTC_OW aktyvuojamas, kai counter'is pasiekia 0 (skaiciuoja COUNT UP)
*/
void rtc_isr(void)
{
    if(rtc_check_flag(RTC_OW)){

        rtc_clear_flag(RTC_OW);

        rtc_set_counter_val(T60SEC);
    }

    if(rtc_check_flag(RTC_SEC)){

        rtc_clear_flag(RTC_SEC);

        rtc_timestamp++;

        UpdateDateTimeRequired = true;
    }
}



