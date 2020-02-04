
#include "globals.h"
#include "rtc.h"


static struct _dt datetime = {0};
volatile uint32_t timestamp;

volatile uint8_t UpdateDateTimeRequired = false;


const char wday_array_en[7][10] = {"Sunday", "Monday", "Thusday", "Wednesday", "Thusday", "Friday", "Saturday"};
const char wday_array_lt[7][15] = {"Sekmadienis", "Pirmadienis", "Antradienis", "Treciadienis", "Ketvirtadienis", "Penktadienis", "Sestadienis"};

const char mon_arr_en[12][10] = {"January", "February", "March", "April", "May", "June", "July", "August", "September", "October", "November", "December"};
const char mon_arr_lt[12][10] = {"Sausis", "Vasaris", "Kovas", "Balandis", "Geguze", "Birzelis", "Liepa", "Rugpjutis", "Rugsejis", "Spalis", "Lapkritis", "Gruodis"};
const uint8_t days_in_month[12] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};


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
        pwr_disable_backup_domain_write_protect();

        /* nustatom defoltines reiksmes datos ir laiko */
        datetime.Seconds = 0;
        datetime.Minutes = 0;
        datetime.Hours = 0;
        datetime.WeekDay = 3;   // treciadienis
        datetime.Day = 1;
        datetime.Month = 1;     // sausis
        datetime.Year = 20;

        BKP_USER_REGISTER1 = 0xAA55;
        pwr_enable_backup_domain_write_protect();
    }
}


/* grazinam pointeri i datetime struktura */
struct _dt* rtc_datetime_init(uint32_t rtc_cnt)
{

    /*
    1. Reikia grazinti is EEPROM datetime strukturos turini
    2. Reikia perskaiciuoti RTC kounteri i datos delta ir prideti ja prie nuskaitytos is EEPROM datos
    */


    datetime.Seconds = 30;//(rtc_cnt%3600)%60;
    datetime.Minutes = 59;//(rtc_cnt%3600)/60;
    datetime.Hours = 23;//rtc_cnt/3600;
    datetime.WeekDay = 3;
    datetime.Day = 31;
    datetime.Month = 11;
    datetime.Year = 20;

    if( !((2000 + datetime.Year)%400) ) datetime.IsLeapYear = true;
    else
    {
        if( !((2000 + datetime.Year)%100) ) datetime.IsLeapYear = false;
        else
        {
            if( !((2000 + datetime.Year)%4) ) datetime.IsLeapYear = true;
            else
            {
                datetime.IsLeapYear = false;
            }
        }
    }

    return &datetime;
};



/*  */
int rtc_time_process(void)
{
    if(datetime.Seconds < 59) datetime.Seconds++;
    else
    {
        datetime.Seconds = 0;

        if(datetime.Minutes < 59) datetime.Minutes++;
        else
        {
            datetime.Minutes = 0;

            if(datetime.Hours < 23) datetime.Hours++;
            else
            {
                datetime.Hours = 0;
                rtc_date_process();
            }
        }
    }

    return 0;
}




/*  */
int rtc_date_process(void)
{
    timestamp = rtc_get_counter_val();

    if(datetime.WeekDay < 6) datetime.WeekDay++;
    else datetime.WeekDay = 0;

    switch(days_in_month[datetime.Month])
    {
    case 31:

        if(datetime.Day < 31) datetime.Day++;
        else
        {
            datetime.Day = 1;

            if(datetime.Month == 11)
            {
                datetime.Month = 0;
                datetime.Year++;

                if(datetime.IsLeapYear == true) datetime.IsLeapYear = false;
            }
            else
            {
                datetime.Month++;
            }
        }
        break;
    case 30:

        if(datetime.Day < 30) datetime.Day++;
        else
        {
            datetime.Day = 1;
            datetime.Month++;
        }
        break;
    default:

        if(datetime.Day < 28) datetime.Day++;
        else
        {
            if(datetime.Day == 28)
            {
                if(datetime.IsLeapYear == true)
                {
                    datetime.Day++;
                }
                else
                {
                    datetime.Day = 1;
                    datetime.Month++;
                }
            }
            else
            {
                datetime.Day = 1;
                datetime.Month++;
            }
        }

        break;
    }

    //rtc_set_counter_val(0);

    return 0;
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



