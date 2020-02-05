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


#endif /* GLOBALS_H_INCLUDED */
