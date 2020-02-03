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


extern volatile uint32_t rtc_timestamp;
extern volatile uint8_t UpdateDateTimeRequired;

/*   */
void    rtc_init(void);
int     rtc_datetime_process(void);

#endif /* RTC_H_INCLUDED */
