#ifndef IIC_H_INCLUDED
#define IIC_H_INCLUDED

#include <libopencm3/cm3/systick.h>
#include <libopencm3/stm32/rcc.h>
#include <libopencm3/stm32/i2c.h>
#include <libopencm3/stm32/gpio.h>

#include "FreeRTOS.h"
#include "task.h"

#include "defs.h"

#define     IIC_DELAY_MS 5
#define     wait()  portYIELD()

//#define     I2C_REQUEST_WRITE 0
//#define     I2C_REQUEST_READ  1

#define MEM_ADDR_WIDTH_8BIT      1
#define MEM_ADDR_WIDTH_16BIT     2

typedef struct{
    uint32_t        handle;
    uint8_t         i2c_addr;
    uint8_t         mem_addr_width;
    uint8_t         alive;
}i2c_dev_t;


void i2c_hw_init( uint32_t i2c );
int iic_write( i2c_dev_t *dev, uint16_t reg, uint8_t *data, uint16_t len );
int iic_read( i2c_dev_t *dev, uint16_t reg, uint8_t *data, uint16_t len );

#endif /* IIC_H_INCLUDED */
