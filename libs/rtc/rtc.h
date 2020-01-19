#ifndef RTC_H_INCLUDED
#define RTC_H_INCLUDED

#include <libopencm3/stm32/rcc.h>
#include <libopencm3/stm32/gpio.h>
#include <libopencm3/cm3/nvic.h>
#include <libopencm3/stm32/rtc.h>
#include <libopencm3/stm32/f1/bkp.h>



#define rtc_isr_handler RTC_IRQHandler



void rtc_init(void);
void rtc_handler(void);


#endif /* RTC_H_INCLUDED */
