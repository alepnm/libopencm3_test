#ifndef RTC_H_INCLUDED
#define RTC_H_INCLUDED

#include <libopencm3/stm32/rcc.h>
#include <libopencm3/stm32/gpio.h>
#include <libopencm3/cm3/nvic.h>
#include <libopencm3/stm32/rtc.h>
#include <libopencm3/stm32/f1/bkp.h>


/* backup registers                                 MSB  |  LSB */
#define BKP_USER_REGISTER1  BKP_DR1     //  init register|   ...
#define BKP_USER_REGISTER2  BKP_DR2     //     seconds   | leap_year:year
#define BKP_USER_REGISTER3  BKP_DR3     //     minutes   |    hours
#define BKP_USER_REGISTER4  BKP_DR4     //       day     |  weekday:month
// .....
#define BKP_USER_REGISTER20 BKP_DR20


extern volatile uint32_t timestamp;
extern volatile uint8_t UpdateDateTimeRequired;

/*   */
void    rtc_init(void);

#endif /* RTC_H_INCLUDED */
