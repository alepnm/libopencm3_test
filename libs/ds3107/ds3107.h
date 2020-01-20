#ifndef DS3107_H_INCLUDED
#define DS3107_H_INCLUDED

#include "defs.h"
#include "iic.h"

#define RTC_IRQHandler rtc_irq_handler

#define CONVERT_BIN2BCD(__VALUE__) (uint8_t)((((__VALUE__) / 10U) << 4U) | ((__VALUE__) % 10U))
#define CONVERT_BCD2BIN(__VALUE__) (uint8_t)(((uint8_t)((__VALUE__) & (uint8_t)0xF0U) >> (uint8_t)0x4U) * 10U + ((__VALUE__) & (uint8_t)0x0FU))


/* RTC */
typedef struct{

    i2c_dev_t   i2c_base;

    struct _datetime{
        uint8_t Seconds;
        uint8_t Minutes;
        uint8_t Hours;
        uint8_t WeekDay;
        uint8_t Date;
        uint8_t Month;
        uint8_t Year;
    }datetime;

    uint8_t one_sec_flag;
    uint8_t need_to_update_datetime_flag;
    uint8_t datetime_regs[7];
    uint8_t bkp_regs[56];
}rtc_t;

extern rtc_t* rtc;

int ds_init          ( rtc_t* dev );
int ds_get_datetime  ( rtc_t* dev );
int ds_set_datetime  ( rtc_t* dev, uint8_t year, uint8_t month, uint8_t date, uint8_t weekday, uint8_t hour, uint8_t minute, uint8_t second );


#endif /* DS3107_H_INCLUDED */
