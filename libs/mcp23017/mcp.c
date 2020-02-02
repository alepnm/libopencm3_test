
//#include <libopencm3/stm32/i2c.h>

#include "mcp.h"

/* Port Expander MCP23017 */
#define MCP_I2C_ADDR        (0x40>>1)

typedef enum {
    MCP_IODIRA =    0x00,
    MCP_IODIRB =    0x01,
    MCP_IPOLA =     0x02,
    MCP_IPOLB =     0x03,
    MCP_GPINTENA =  0x04,
    MCP_GPINTENB =  0x05,
    MCP_DEFVALA =   0x06,
    MCP_DEFVALB =   0x07,
    MCP_INTCONA =   0x08,
    MCP_INTCONB =   0x09,
    MCP_IOCON =     0x0A,
    MCP_IOCONB =    0x0B,
    MCP_GPPUA =     0x0C,
    MCP_GPPUB =     0x0D,
    MCP_INTFA =     0x0E,
    MCP_INTFB =     0x0F,
    MCP_INTCAPA =   0x10,
    MCP_INTCAPB =   0x11,
    MCP_GPIOA =     0x12,
    MCP_GPIOB =     0x13,
    MCP_OLATA =     0x14,
    MCP_OLATB =     0x15
} mcp_reg_t;

#define MCP_PORTA           MCP_GPIOA
#define MCP_PORTB           MCP_GPIOB

#define ALL_INPUTS          0xFF
#define ALL_OUTPUTS         0x00

static mcp_t mcp_dev;
mcp_t* mcp = &mcp_dev;


/* MCP23017 funkcijos  */
static int mcp_read_register( mcp_t* dev, mcp_reg_t reg, uint8_t* data ){

    return iic_read( &(dev->i2c_base), (uint8_t)reg, data, 1 );
}

static int mcp_write_register( mcp_t* dev, mcp_reg_t reg, uint8_t data ){

    return iic_write( &(dev->i2c_base), (uint8_t)reg, &data, 1 );
}

int mcp_read_port( mcp_t* dev, uint8_t port, uint8_t* data ){

    return mcp_read_register( dev, port, data );
}

int mcp_write_port( mcp_t* dev, uint8_t port, uint8_t data ){

    return mcp_write_register( dev, port, data );
}

int mcp_read_pin( mcp_t* dev, iopin_t* pin_struct, uint8_t* state ){

    uint8_t temp = 0;

    (void)mcp_read_port( dev, pin_struct->port, &temp );

    *state = READ_BIT(temp>>pin_struct->pin, 0x01);

    return 0;
}

int mcp_write_pin( mcp_t* dev, iopin_t* pin_struct, uint8_t state ){

    uint8_t temp = 0;

    (void)mcp_read_port( dev, pin_struct->port, &temp );

    if(state){
        SET_BIT(temp, 1<<pin_struct->pin);
    }else{
        CLEAR_BIT(temp, 1<<pin_struct->pin);
    }

    (void)mcp_write_port( dev, pin_struct->port, temp );

    return 0;
}

void mcp_irq_handler(void){


}

int mcp_init( mcp_t* dev ){

    i2c_dev_t* i2c = &(dev->i2c_base);

    i2c->handle = I2C1;
    i2c->i2c_addr = MCP_I2C_ADDR;
    i2c->mem_addr_width = MEM_ADDR_WIDTH_8BIT;
    i2c->alive = 0;
    dev->devid = 0;

    i2c_hw_init(i2c->handle);

    (void)iic_check( &(dev->i2c_base) );

    (void)mcp_write_register( dev, MCP_IOCON, 0x00 );
    (void)mcp_write_register( dev, MCP_IODIRA, ALL_OUTPUTS );
    (void)mcp_write_register( dev, MCP_IODIRB, ALL_OUTPUTS );
    (void)mcp_write_register( dev, MCP_GPIOA, 0x00 );
    (void)mcp_write_register( dev, MCP_GPIOB, 0x00 );
    (void)mcp_write_register( dev, MCP_OLATA, 0x00 );
    (void)mcp_write_register( dev, MCP_OLATB, 0x00 );

    return 0;
}



