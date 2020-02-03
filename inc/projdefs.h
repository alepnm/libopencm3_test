#ifndef PROJDEFS_H_INCLUDED
#define PROJDEFS_H_INCLUDED


#include <stdint.h>
#include <stdlib.h>

typedef enum { RESET = 0, SET = !RESET } eStatus;
typedef enum { DISABLE = 0, ENABLE = !DISABLE } eState;
typedef enum { SUCCESS = 0, ERROR = !SUCCESS } eResult;

enum { ERR_NONE = 0, ERR_ERROR, ERR_TIMEOUT };
enum { STATE_DISABLED = 0, STATE_ENABLED, STATE_READY, STATE_BUSY, STATE_TIMEOUT };     // steitai

#define IS_FUNCTIONAL_STATE(STATE) (((STATE) == DISABLE) || ((STATE) == ENABLE))



#define SET_BIT(REG, BIT)     ((REG) |= (BIT))
#define CLEAR_BIT(REG, BIT)   ((REG) &= ~(BIT))
#define READ_BIT(REG, BIT)    ((REG) & (BIT))
#define CLEAR_REG(REG)        ((REG) = (0x0))
#define WRITE_REG(REG, VAL)   ((REG) = (VAL))
#define READ_REG(REG)         ((REG))
#define MODIFY_REG(REG, CLEARMASK, SETMASK)  WRITE_REG((REG), (((READ_REG(REG)) & (~(CLEARMASK))) | (SETMASK)))


/* IRQ handlers aliases */
#define sys_tick_handler    SysTick_Handler
#define usart1_isr          USART1_IRQHandler
#define usart2_isr          USART2_IRQHandler
#define usart3_isr          USART3_IRQHandler
#define rtc_isr             RTC_IRQHandler
#define adc1_2_isr          ADC1_2_IRQHandler


#endif /* PROJDEFS_H_INCLUDED */
