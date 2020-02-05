
#include <libopencm3/stm32/rtc.h>
//#include <libopencm3/stm32/f1/bkp.h>


#include "calendar.h"

struct _dt datetime = {0};
static uint8_t SummerTimeCorrectionFlag = false;   // saugiklis nuo pakartotinos laiko korekcijos


const char wday_array_en[7][10] = {"Sunday", "Monday", "Thusday", "Wednesday", "Thusday", "Friday", "Saturday"};
const char wday_array_lt[7][15] = {"Sekmadienis", "Pirmadienis", "Antradienis", "Treciadienis", "Ketvirtadienis", "Penktadienis", "Sestadienis"};

const char mon_arr_en[12][10] = {"January", "February", "March", "April", "May", "June", "July", "August", "September", "October", "November", "December"};
const char mon_arr_lt[12][10] = {"Sausis", "Vasaris", "Kovas", "Balandis", "Geguze", "Birzelis", "Liepa", "Rugpjutis", "Rugsejis", "Spalis", "Lapkritis", "Gruodis"};
const uint8_t days_in_month[12] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};




/* grazinam pointeri i datetime struktura */
int cal_datetime_init(uint32_t rtc_cnt, struct _dt* dt)
{

    /*
    1. Reikia grazinti is EEPROM datetime strukturos turini
    2. Reikia perskaiciuoti RTC kounteri i datos delta ir prideti ja prie nuskaitytos is EEPROM datos
    */


    dt->Seconds = 30;//(rtc_cnt%3600)%60;
    dt->Minutes = 59;//(rtc_cnt%3600)/60;
    dt->Hours = 23;//rtc_cnt/3600;
    dt->WeekDay = 3;
    dt->Day = 31;
    dt->Month = 11;
    dt->Year = 20;

    if( !((2000 + dt->Year)%400) ) dt->IsLeapYear = true;
    else
    {
        if( !((2000 + dt->Year)%100) ) dt->IsLeapYear = false;
        else
        {
            if( !((2000 + dt->Year)%4) ) dt->IsLeapYear = true;
            else
            {
                dt->IsLeapYear = false;
            }
        }
    }

    return 0;
};


/*  */
int cal_date_process(void)
{
    timestamp = rtc_get_counter_val();

    if(datetime.WeekDay < 6) datetime.WeekDay++;
    else datetime.WeekDay = 0;

    switch(days_in_month[datetime.Month])
    {

    case 28:

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

    default:

        if(datetime.Day < 30) datetime.Day++;
        else
        {
            datetime.Day = 1;
            datetime.Month++;
        }

        break;
    }

//rtc_set_counter_val(0);

    return 0;
}


int cal_summer_time_correction(void)
{
    switch(datetime.Month)
    {
    case 2: // kovas
        datetime.Hours -= 1;
        break;
    case 9: // spalis
        datetime.Hours += 1;
        break;
    }

    SummerTimeCorrectionFlag = true;

    return 0;
}







/*  */
int cal_time_process(void)
{
    if(datetime.Seconds < 59) datetime.Seconds++;
    else
    {
        datetime.Seconds = 0;

        if(datetime.Minutes < 59) datetime.Minutes++;
        else
        {
            datetime.Minutes = 0;

            if(datetime.Hours < 23)
            {
                datetime.Hours++;

                /* jai dabar 2:00AM, paskutinis menesio sekmadienis vykdom vasaros laiko korekcija */
                if(datetime.Hours == 2 && SummerTimeCorrectionFlag == false){

                    if( (datetime.Day > (days_in_month[datetime.Month] - 7)) && datetime.WeekDay == 0 )
                    {
                        cal_summer_time_correction();
                    }
                }
            }
            else
            {
                datetime.Hours = 0;
                cal_date_process();

                SummerTimeCorrectionFlag = false;
            }
        }
    }

    rtc_backup_datetime();

    return 0;
}


