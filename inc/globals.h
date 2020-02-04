#ifndef GLOBALS_H_INCLUDED
#define GLOBALS_H_INCLUDED

#include "projdefs.h"
#include "gpio.h"



#define SYS_GPIO_PIN1       (1<<0)
#define SYS_GPIO_PIN2       (1<<1)
#define SYS_GPIO_PIN3       (1<<2)
#define SYS_GPIO_PIN4       (1<<3)
//...
#define SYS_INIT_BIT        (1<<16)


/* Status registras

 */
struct _status{
    uint32_t    status;

}sys_status;

extern  struct _status sys_status;



struct _dt{
    uint8_t Seconds;
    uint8_t Minutes;
    uint8_t Hours;
    uint8_t WeekDay;
    uint8_t Date;
    uint8_t Month;
    uint8_t Year;
};

#endif /* GLOBALS_H_INCLUDED */
