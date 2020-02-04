#ifndef RTC_H_INCLUDED
#define RTC_H_INCLUDED

#include <libopencm3/stm32/rcc.h>
#include <libopencm3/stm32/gpio.h>
#include <libopencm3/cm3/nvic.h>
#include <libopencm3/stm32/rtc.h>
#include <libopencm3/stm32/f1/bkp.h>



/* backup registers */
#define BKP_USER_REGISTER1  BKP_DR1
#define BKP_USER_REGISTER2  BKP_DR2
#define BKP_USER_REGISTER3  BKP_DR3
#define BKP_USER_REGISTER4  BKP_DR4
// .....
#define BKP_USER_REGISTER20 BKP_DR20

extern const char wday_array_en[7][10];
extern const char mon_arr_en[12][10];
extern const char wday_array_lt[7][15];
extern const char mon_arr_lt[12][10];
extern const uint8_t days_in_month[12];


extern volatile uint32_t timestamp;
extern volatile uint8_t UpdateDateTimeRequired;

/*   */
void        rtc_init(void);
struct _dt* rtc_datetime_init(uint32_t rtc_cnt);
int         rtc_time_process(void);
int         rtc_date_process(void);

#endif /* RTC_H_INCLUDED */
