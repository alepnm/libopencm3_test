#ifndef CALENDAR_H_INCLUDED
#define CALENDAR_H_INCLUDED

#include "projdefs.h"

/* Config registro struktura */


/*  */
struct _dt{
    uint8_t Config      :8;
    uint8_t Seconds     :6;
    uint8_t Minutes     :6;
    uint8_t Hours       :5;
    uint8_t WeekDay     :3;
    uint8_t Day         :5;
    uint8_t Month       :4;
    uint8_t Year        :7;
    uint8_t IsLeapYear  :1;
};

extern struct _dt datetime;


extern const char wday_array_en[7][10];
extern const char mon_arr_en[12][10];
extern const char wday_array_lt[7][15];
extern const char mon_arr_lt[12][10];
extern const uint8_t days_in_month[12];


int cal_datetime_init(uint32_t rtc_cnt, struct _dt* dt);
int cal_time_process(void);
int cal_date_process(void);


#endif /* CALENDAR_H_INCLUDED */
