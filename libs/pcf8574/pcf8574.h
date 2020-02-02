#ifndef PCF8574_C_INCLUDED
#define PCF8574_C_INCLUDED

#include "projdefs.h"
#include "iic.h"


#define     PCF8574_I2C_ADDRESS    (0x40>>1)

/*
Portu struktura: Quasi-bidirectional I/Os.

PCF8574 konfiguravimas:
    jai i atitinkamo pino bita irasyti 1, jis dirbs kaip INPUT. Is jo galima skaityti jo busena;
    jai pinas turi dirbti kaip OUTPUT, i ji tiesiog rasom 0/1. Isejime turesim atitinkama lygi;

    pcf8574_Write(0xFF);    // <-- visi pinai kaip INPUT, arba visi pinai kaip OUTPUT ir ju isejime turim 1
    pcf8574_Write(0x00);    // <-- visi pinai kaip OUTPUT ir ju isejime turim 0
*/


typedef struct{

    i2c_dev_t   i2c_base;

    uint8_t     devid;
    uint8_t     port_data;

}pcf_t;


void    pcf8574_init(pcf_t* dev);
void    pcf8574_read_port(pcf_t* dev, uint8_t* data);
void    pcf8574_write_port(pcf_t* dev, uint8_t data);
void    pcf8574_write_pin(pcf_t* dev, uint8_t pin, uint8_t state);





#endif /* PCF8574_C_INCLUDED */
