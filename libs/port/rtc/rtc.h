#ifndef RTC_H_INCLUDED
#define RTC_H_INCLUDED


#include <libopencm3/stm32/rcc.h>
#include <libopencm3/stm32/gpio.h>
#include <libopencm3/cm3/nvic.h>
#include <libopencm3/stm32/rtc.h>
#include <libopencm3/stm32/f1/bkp.h>



#define rtc_isr_handler RTC_IRQHandler


/* backup registers */
#define BKP_USER_REGISTER1  BKP_DR1
#define BKP_USER_REGISTER2  BKP_DR2
#define BKP_USER_REGISTER3  BKP_DR3
#define BKP_USER_REGISTER4  BKP_DR4
// .....
#define BKP_USER_REGISTER42 BKP_DR42


/*   */
void rtc_init(void);
void rtc_process(void);


/*   */
int bkp_read_register(uint32_t reg, uint16_t* data);
int bkp_write_register(uint32_t reg, uint16_t data);


#endif /* RTC_H_INCLUDED */
