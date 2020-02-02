#ifndef EEP_H_INCLUDED
#define EEP_H_INCLUDED

#include "projdefs.h"
#include "iic.h"

/* EEPROM */
typedef struct{

    i2c_dev_t   i2c_base;

    uint8_t     devid;
    uint16_t    size_bytes;
    uint8_t     page_size;
}eeprom_t;

extern eeprom_t* eeprom;

int eep_init        ( eeprom_t* dev );
int eep_read        ( eeprom_t* dev, uint16_t reg, uint8_t* data, uint16_t len );
int eep_read_byte   ( eeprom_t* dev, uint16_t reg, uint8_t* data );
int eep_read_word   ( eeprom_t* dev, uint16_t reg, uint16_t* data );
int eep_read_dword  ( eeprom_t* dev, uint16_t reg, uint32_t* data );
int eep_write       ( eeprom_t* dev, uint16_t reg, uint8_t *data, uint16_t len );
int eep_write_byte  ( eeprom_t* dev, uint16_t reg, uint8_t data );
int eep_write_word  ( eeprom_t* dev, uint16_t reg, uint16_t data );
int eep_write_dword ( eeprom_t* dev, uint16_t reg, uint32_t data );
int eep_clear       ( eeprom_t* dev );

#endif /* EEP_H_INCLUDED */
