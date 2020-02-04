
#include "globals.h"
#include "rtc.h"


static struct _dt datetime = {0};

volatile uint8_t UpdateDateTimeRequired = false;

uint8_t days_in_month[12] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};



void rtc_init(void)
{
    rtc_awake_from_off(RCC_LSE);

    rtc_datetime_init( rtc_get_counter_val() );

    rtc_set_prescale_val(32768);        // 1 sec
    rtc_set_counter_val(0);

	nvic_enable_irq(NVIC_RTC_IRQ);
	nvic_set_priority(NVIC_RTC_IRQ, 1);

    rtc_clear_flag(RTC_OW);
    rtc_clear_flag(RTC_ALR);
    rtc_clear_flag(RTC_SEC);

    rtc_interrupt_disable(RTC_ALR);
	rtc_interrupt_enable(RTC_SEC);
	rtc_interrupt_disable(RTC_OW);


    /* backup registrai */
	if(BKP_USER_REGISTER1 != 0xAA55){

        pwr_disable_backup_domain_write_protect();


        BKP_USER_REGISTER1 = 0xAA55;
        pwr_enable_backup_domain_write_protect();
	}

}


/* grazinam pointeri i datetime struktura */
struct _dt* rtc_datetime_init(uint32_t rtc_cnt){

/*
1. Reikia grazinti is EEPROM datetime strukturos turini
2. Reikia perskaiciuoti RTC kounteri i datos delta ir prideti ja prie nuskaitytos is EEPROM datos
*/


    datetime.Seconds = (rtc_cnt%3600)%60;
    datetime.Minutes = (rtc_cnt%3600)/60;
    datetime.Hours = rtc_cnt/3600;
    datetime.WeekDay = 3;
    datetime.Date = 1;
    datetime.Month = 1;
    datetime.Year = 20;


    return &datetime;
};



/*  */
int rtc_datetime_process(void){






    rtc_set_counter_val(0);

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
    }

    if(rtc_check_flag(RTC_SEC)){

        rtc_clear_flag(RTC_SEC);

        UpdateDateTimeRequired = true;
    }
}



