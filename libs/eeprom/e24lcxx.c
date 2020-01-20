
#include "e24lcxx.h"

/* EEPROM */
#define E24LC32

/* EEPROM 24LC02 */
#ifdef E24LC02
#define EE_I2C_ADDR         (0xA0>>1)
#define EE_PAGE_SIZE        8
#define EE_MEM_ADDR_WIDTH   MEM_ADDR_WIDTH_8BIT
#define EE_SIZE_BYTES       (2048/8)
#endif

/* EEPROM 24LC04 */
#ifdef E24LC04
#define EE_I2C_ADDR         (0xA0>>1)
#define EE_PAGE_SIZE        16
#define EE_MEM_ADDR_WIDTH   MEM_ADDR_WIDTH_16BIT
#define EE_SIZE_BYTES       (4096/8)
#endif

/* EEPROM 24LC08 */
#ifdef E24LC08
#define EE_I2C_ADDR         (0xA0>>1)
#define EE_PAGE_SIZE        16
#define EE_MEM_ADDR_WIDTH   MEM_ADDR_WIDTH_16BIT
#define EE_SIZE_BYTES       (8192/8)
#endif

/* EEPROM 24LC16 */
#ifdef E24LC16
#define EE_I2C_ADDR         (0xA0>>1)
#define EE_PAGE_SIZE        16
#define EE_MEM_ADDR_WIDTH   MEM_ADDR_WIDTH_16BIT
#define EE_SIZE_BYTES       (16384/8)
#endif

/* EEPROM 24LC32 */
#ifdef E24LC32
#define EE_I2C_ADDR         (0xA0>>1)
#define EE_PAGE_SIZE        64  // 8 pages x 8 byte - greitas rasymas (buferis gali buti uzpildytas vienu irasymu,
                                // bet po STOP reikia laukti, kol irasys visus buferio peidzus, su 5ms peidzo irasymo pause)
#define EE_MEM_ADDR_WIDTH   MEM_ADDR_WIDTH_16BIT
#define EE_SIZE_BYTES       (32768/8)
#endif

/* EEPROM 24LC64 */
#ifdef E24LC64
#define EE_I2C_ADDR         (0xA0>>1)
#define EE_PAGE_SIZE        32
#define EE_MEM_ADDR_WIDTH   MEM_ADDR_WIDTH_16BIT
#define EE_SIZE_BYTES       (65536/8)
#endif

static eeprom_t eep_dev;
eeprom_t* eeprom = &eep_dev;

/*  EEPROM funkcijos  */
static int eep_check( eeprom_t* dev ){

    //return iic_check( &(dev->i2c_base) );
    dev->i2c_base.alive = 1;

    return 0;
}

int eep_read( eeprom_t* dev, uint16_t reg, uint8_t* data, uint16_t len ){

    i2c_dev_t* i2c = &(dev->i2c_base);

    if(!i2c->alive) return -1;

    return iic_read( i2c, reg, data, len );
}

int eep_read_byte( eeprom_t* dev, uint16_t reg, uint8_t* data ){

    return iic_read( &(dev->i2c_base), reg, data, 1 );
}

int eep_read_word( eeprom_t* dev, uint16_t reg, uint16_t* data ){

    return iic_read( &(dev->i2c_base), reg, (uint8_t*)data, 2 );
}

int eep_read_dword( eeprom_t* dev, uint16_t reg, uint32_t* data ){

    return iic_read( &(dev->i2c_base), reg, (uint8_t*)data, 4 );
}

#ifdef E24LC32
int eep_write( eeprom_t* dev, uint16_t reg, uint8_t *data, uint16_t len ){

    i2c_dev_t* i2c = &(dev->i2c_base);

    if(!i2c->alive) return -1;

    uint8_t offset = reg % dev->page_size;  // baitas nuo peidzo pradzios

    uint8_t bytes_to_send = dev->page_size - offset;

    if(len < bytes_to_send) bytes_to_send = len;

    uint8_t pages = bytes_to_send % 8;

    while(len)
    {
        if( iic_write( i2c, reg, data, bytes_to_send ) == -1 ) return -1;

        len -= bytes_to_send;
        data += bytes_to_send;
        reg += bytes_to_send;

        bytes_to_send = (len > dev->page_size) ? dev->page_size : len;

        vTaskDelay(IIC_DELAY_MS * pages);
    }

    return 0;
}
#else
int eep_write( eeprom_t* dev, uint16_t reg, uint8_t *data, uint16_t len ){

    i2c_dev_t* i2c = &(dev->i2c_base);

    if(!i2c->alive) return -1;

    uint8_t offset = reg % dev->page_size;  // baitas nuo peidzo pradzios

    uint8_t bytes_to_send = dev->page_size - offset;

    if(len < bytes_to_send) bytes_to_send = len;

    while(len)
    {
        if( iic_write( i2c, reg, data, bytes_to_send ) == -1 ) return -1;

        len -= bytes_to_send;
        data += bytes_to_send;
        reg += bytes_to_send;

        bytes_to_send = (len > dev->page_size) ? dev->page_size : len;

        vTaskDelay(IIC_DELAY_MS);
    }

    return 0;
}
#endif

int eep_write_byte( eeprom_t* dev, uint16_t reg, uint8_t data ){

    return eep_write( dev, reg, &data, 1 );
}

int eep_write_word( eeprom_t* dev, uint16_t reg, uint16_t data ){

    return eep_write( dev, reg, (uint8_t*)&data, 2 );
}

int eep_write_dword( eeprom_t* dev, uint16_t reg, uint32_t data ){

    return eep_write( dev, reg, (uint8_t*)&data, 4 );
}

int eep_clear( eeprom_t* dev ){

    uint8_t *data = (uint8_t*)malloc(sizeof(uint8_t)*(dev->page_size));
    uint16_t i = 0;

    if(!dev->i2c_base.alive) return -1;

    do
    {
        *(data+i) = 0xFF;
    }
    while(++i < dev->page_size);

    i = 0;

    while(i < dev->size_bytes)
    {
        (void)eep_write( dev, i, data, dev->page_size );
        i += dev->page_size;
    }

    free(data);

    return 0;
}

int eep_init( eeprom_t* dev ){

    i2c_dev_t* i2c = &(dev->i2c_base);

    i2c->handle = I2C1;
    i2c->i2c_addr = EE_I2C_ADDR;
    i2c->mem_addr_width = EE_MEM_ADDR_WIDTH;
    i2c->alive = 0;
    dev->size_bytes = EE_SIZE_BYTES;
    dev->page_size = EE_PAGE_SIZE;
    dev->devid = 0;

    i2c_hw_init(i2c->handle);

    return eep_check(dev);
}

