
#include "globals.h"
#include "rtc.h"
#include "calendar.h"


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
    if(BKP_USER_REGISTER1 != 0xA500)
    {
        rtc_set_counter_val(0);

        pwr_disable_backup_domain_write_protect();

        /* cia reikia sukonfiguruoti BKP registrus - irasyti defoltinius datetime */
        datetime.Seconds = 0;
        datetime.Minutes = 0;
        datetime.Hours = 0;
        datetime.WeekDay = 3;
        datetime.Day = 1;
        datetime.Month = 0;
        datetime.Year = 20;
        datetime.IsLeapYear = 1;

        BKP_USER_REGISTER1 = MAKE_INT16(0xA5, 0);

        rtc_backup_datetime();

        pwr_enable_backup_domain_write_protect();
    }

    timestamp = rtc_get_counter_val();

    datetime.Seconds = (uint8_t)((BKP_USER_REGISTER2&0x3F00)>>8);
    datetime.Minutes = (uint8_t)((BKP_USER_REGISTER3&0x3F00)>>8);
    datetime.Hours = (uint8_t)(BKP_USER_REGISTER3&0x001F);
    datetime.WeekDay = (uint8_t)((BKP_USER_REGISTER4&0x00E0)>>5);
    datetime.Day = (uint8_t)((BKP_USER_REGISTER4&0x1F00)>>8);
    datetime.Month = (uint8_t)(BKP_USER_REGISTER4&0x000F);
    datetime.Year = (uint8_t)(BKP_USER_REGISTER2&0x007F);
    datetime.IsLeapYear = (uint8_t)((BKP_USER_REGISTER2&0x0080)>>7);


}


void rtc_backup_datetime(void)
{
    uint8_t tmp = ((datetime.IsLeapYear<<7)&0x80) | (datetime.Year&0x7F);

    BKP_USER_REGISTER2 = MAKE_INT16(datetime.Seconds, tmp);
    BKP_USER_REGISTER3 = MAKE_INT16(datetime.Minutes, datetime.Hours);

    tmp = ((datetime.WeekDay<<5)&0xE0) | (datetime.Month&0x0F);

    BKP_USER_REGISTER4 = MAKE_INT16(datetime.Day, tmp);
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



