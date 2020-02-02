#ifndef IIC_H_INCLUDED
#define IIC_H_INCLUDED

#include <libopencm3/cm3/systick.h>
#include <libopencm3/stm32/rcc.h>
#include <libopencm3/stm32/i2c.h>
#include <libopencm3/stm32/gpio.h>

#include "FreeRTOS.h"
#include "task.h"

#include "projdefs.h"
#include "gpio.h"
#include "tim.h"

#define     IIC_DELAY_MS 5
#define     wait()  //portYIELD()

#define MEM_ADDR_WIDTH_8BIT      1
#define MEM_ADDR_WIDTH_16BIT     2

typedef struct{
    uint32_t        handle;
    uint8_t         i2c_addr;
    uint8_t         mem_addr_width;
    uint8_t         alive;
}i2c_dev_t;


void        i2c_hw_init( uint32_t i2c );
int         iic_check( i2c_dev_t *dev ) __attribute__((unused));
int         iic_write( i2c_dev_t *dev, uint16_t reg, uint8_t *data, uint16_t len );
int         iic_read( i2c_dev_t *dev, uint16_t reg, uint8_t *data, uint16_t len );

/* Paprastas siuntimas/skaitymas be registro adreso irasymo */
int         iic_simple_rw( i2c_dev_t *dev, uint8_t *data, uint16_t len, uint8_t rw ) __attribute__((unused));
uint8_t     iic_get_init_flag(uint32_t i2c) __attribute__((unused));

#endif /* IIC_H_INCLUDED */
