#ifndef DS1307_H_INCLUDED
#define DS1307_H_INCLUDED

#include "globals.h"
#include "iic.h"

#define CONVERT_BIN2BCD(__VALUE__) (uint8_t)((((__VALUE__) / 10U) << 4U) | ((__VALUE__) % 10U))
#define CONVERT_BCD2BIN(__VALUE__) (uint8_t)(((uint8_t)((__VALUE__) & (uint8_t)0xF0U) >> (uint8_t)0x4U) * 10U + ((__VALUE__) & (uint8_t)0x0FU))


/* RTC */
typedef struct{

    i2c_dev_t   i2c_base;
    struct _dt  datetime;
    uint8_t     one_sec_flag;
    uint8_t     need_to_update_datetime_flag;
    uint8_t     datetime_regs[7];
    uint8_t     bkp_regs[56];
}rtc_t;

extern rtc_t* ds1307;

int ds_init          ( rtc_t* dev );
int ds_get_datetime  ( rtc_t* dev );
int ds_set_datetime  ( rtc_t* dev, uint8_t year, uint8_t month, uint8_t date, uint8_t weekday, uint8_t hour, uint8_t minute, uint8_t second );


#endif /* DS1307_H_INCLUDED */
