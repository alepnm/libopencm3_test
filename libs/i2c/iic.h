#ifndef IIC_H_INCLUDED
#define IIC_H_INCLUDED


#define MEM_ADDR_WIDTH_8BIT      1
#define MEM_ADDR_WIDTH_16BIT     2


#define RTC_IRQHandler rtc_irq_handler

typedef struct{
    uint32_t        handle;
    uint8_t         i2c_addr;
    uint8_t         devid;
    uint8_t         mem_addr_width;
    uint8_t         alive;
}i2c_dev_t;


/* EEPROM */
typedef struct{

    i2c_dev_t   i2c_base;

    uint16_t    size_bytes;
    uint8_t     page_size;
}eeprom_t;

extern eeprom_t* eeprom;


/* RTC */
typedef struct{

    i2c_dev_t   i2c_base;

    struct _datetime{
        uint8_t Seconds;
        uint8_t Minutes;
        uint8_t Hours;
        uint8_t WeekDay;
        uint8_t Date;
        uint8_t Month;
        uint8_t Year;
    }datetime;

    uint8_t one_sec_flag;
    uint8_t need_to_update_datetime_flag;
    uint8_t datetime_regs[7];
    uint8_t bkp_regs[56];
}rtc_t;

extern rtc_t* rtc;


/* MCP */
typedef struct{
    uint8_t port;
    uint8_t pin;
}mcp_pin_t;


typedef struct{

    i2c_dev_t   i2c_base;

    struct{
        const mcp_pin_t OC1;
        const mcp_pin_t OC2;
        const mcp_pin_t OC3;
        const mcp_pin_t OC4;
        const mcp_pin_t OC5;
        const mcp_pin_t OC6;
        const mcp_pin_t OC7;
        const mcp_pin_t OC8;
        const mcp_pin_t OC9;
        const mcp_pin_t ACON;
        const mcp_pin_t MA;
        const mcp_pin_t MB;
        const mcp_pin_t M1ENA;
        const mcp_pin_t M2ENA;
        const mcp_pin_t M3ENA;
        const mcp_pin_t M4ENA;
    }Outputs;

    struct{

    }Inputs;

}mcp_t;

extern mcp_t* mcp;



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



int dsrtc_init          ( rtc_t* dev );
int dsrtc_get_datetime  ( rtc_t* dev );
int dsrtc_set_datetime  ( rtc_t* dev, uint8_t year, uint8_t month, uint8_t date, uint8_t weekday, uint8_t hour, uint8_t minute, uint8_t second );


#endif /* IIC_H_INCLUDED */
