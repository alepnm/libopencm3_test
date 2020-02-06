
#include <libopencm3/stm32/rcc.h>
#include <libopencm3/stm32/gpio.h>
#include <libopencm3/cm3/nvic.h>
#include <libopencm3/stm32/rtc.h>
#include <libopencm3/stm32/f1/bkp.h>

#include "globals.h"
#include "rtc.h"
#include "calendar.h"

/* backup registers                                 MSB         |       LSB */
#define BKP_USER_REGISTER1  BKP_DR1     //    init register[8]  |      config[8]
#define BKP_USER_REGISTER2  BKP_DR2     //       seconds[6]     | leap_year[1]:year[7]
#define BKP_USER_REGISTER3  BKP_DR3     //       minutes[6]     |       hours[5]
#define BKP_USER_REGISTER4  BKP_DR4     //         day[5]       |  weekday[3]:month[5]
#define BKP_USER_REGISTER5  BKP_DR5     //                 wtime_minutes
#define BKP_USER_REGISTER6  BKP_DR6
// .....
#define BKP_USER_REGISTER20 BKP_DR20


#define CAL_INIT_REG_MSK    0xFF00
#define CAL_CONFIG_REG_MSK  0x00FF
#define CAL_SECOND_MSK      0x3F00
#define CAL_MINUTE_MSK      0x3F00
#define CAL_HOUR_MSK        0x001F
#define CAL_DAY_MSK         0x1F00
#define CAL_MONTH_MSK       0x000F
#define CAL_YEAR_MSK        0x007F
#define CAL_WEEKDAY_MSK     0x00E0
#define CAL_LEAPYEAR_MSK    0x0080



volatile uint8_t UpdateDateTimeRequired = false;


/*  */
int rtc_backup_datetime(void)
{
    pwr_disable_backup_domain_write_protect();

    BKP_USER_REGISTER1 = (BKP_USER_REGISTER1&0xFF00)    | (datetime.Config&0x00FF);
    BKP_USER_REGISTER2 = ((datetime.Seconds<<8)&0x3F00) | ((datetime.IsLeapYear<<7)&0x0080) | (datetime.Year&0x007F);
    BKP_USER_REGISTER3 = ((datetime.Minutes<<8)&0x3F00) | (datetime.Hours&0x001F);
    BKP_USER_REGISTER4 = ((datetime.Day<<8)&0x1F00)     | ((datetime.WeekDay<<5)&0x00E0)    | (datetime.Month&0x000F);
    BKP_USER_REGISTER5 = sys_status.wtime;

    pwr_enable_backup_domain_write_protect();

    return SUCCESS;
}


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
    if( (uint8_t)((BKP_USER_REGISTER1&0xFF00)>>8) != 0xA5 )
    {
        rtc_set_counter_val(0);

        pwr_disable_backup_domain_write_protect();

        BKP_USER_REGISTER1 = 0;
        BKP_USER_REGISTER2 = 0;
        BKP_USER_REGISTER3 = 0;
        BKP_USER_REGISTER4 = 0;
        BKP_USER_REGISTER5 = 0;
        BKP_USER_REGISTER6 = 0;

        /* cia reikia sukonfiguruoti BKP registrus - irasyti defoltinius datetime */
        datetime.Config = 0;
        datetime.Seconds = 0;
        datetime.Minutes = 40;
        datetime.Hours = 22;
        datetime.WeekDay = 4;
        datetime.Day = 6;
        datetime.Month = 1;
        datetime.Year = 20;
        datetime.IsLeapYear = 1;

        sys_status.wtime = 0;

        BKP_USER_REGISTER1 = MAKE_INT16(0xA5, datetime.Config);

        rtc_backup_datetime();

        pwr_enable_backup_domain_write_protect();
    }

    /* pirma pasiimam datetime is backup registru, poto padarom jos korekcija pagal rtc kounteri */
    datetime.Config = (uint8_t)(BKP_USER_REGISTER1&0x00FF);
    datetime.Seconds = (uint8_t)((BKP_USER_REGISTER2&0x3F00)>>8);
    datetime.Minutes = (uint8_t)((BKP_USER_REGISTER3&0x3F00)>>8);
    datetime.Hours = (uint8_t)(BKP_USER_REGISTER3&0x001F);
    datetime.WeekDay = (uint8_t)((BKP_USER_REGISTER4&0x00E0)>>5);
    datetime.Day = (uint8_t)((BKP_USER_REGISTER4&0x1F00)>>8);
    datetime.Month = (uint8_t)(BKP_USER_REGISTER4&0x000F);
    datetime.Year = (uint8_t)(BKP_USER_REGISTER2&0x007F);
    datetime.IsLeapYear = (uint8_t)((BKP_USER_REGISTER2&0x0080)>>7);

    sys_status.wtime = BKP_USER_REGISTER5;

    uint32_t tmp = rtc_get_counter_val();

    while(tmp >= 86400){

        tmp -= 86400;

        cal_date_update(); // pakoreguojam datatime vienai dienai pagal kounteri
    }


    /* reikia koreguoti laika pagal likusia kounterio laika (sekundes); gal reikia manipuliuoti ne sekundemis,
    o minutemis??? */
    while(tmp--){

        cal_time_update(); // pakoreguojam datatime vienai sekundei pagal kounteri
    }


    /* pakoreguota datetime issaugojam backup registruose */
    rtc_backup_datetime();
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

        UpdateDateTimeRequired = true;
    }
}



