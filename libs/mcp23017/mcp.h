#ifndef MCP_H_INCLUDED
#define MCP_H_INCLUDED

#include "projdefs.h"
#include "iic.h"

/* MCP */
typedef struct{

    i2c_dev_t   i2c_base;

    uint8_t     devid;

    struct{
        const iopin_t   OC1;
        const iopin_t   OC2;
        const iopin_t   OC3;
        const iopin_t   OC4;
        const iopin_t   OC5;
        const iopin_t   OC6;
        const iopin_t   OC7;
        const iopin_t   OC8;
        const iopin_t   OC9;
        const iopin_t   ACON;
        const iopin_t   MA;
        const iopin_t   MB;
        const iopin_t   M1ENA;
        const iopin_t   M2ENA;
        const iopin_t   M3ENA;
        const iopin_t   M4ENA;
    }Outputs;

    struct{

    }Inputs;

}mcp_t;

extern mcp_t* mcp;

int mcp_init( mcp_t* dev );
int mcp_read_port( mcp_t* dev, uint8_t port, uint8_t* data );
int mcp_write_port( mcp_t* dev, uint8_t port, uint8_t data );
int mcp_read_pin( mcp_t* dev, iopin_t* pin_struct, uint8_t* state );
int mcp_write_pin( mcp_t* dev, iopin_t* pin_struct, uint8_t state );


#endif /* MCP_H_INCLUDED */
