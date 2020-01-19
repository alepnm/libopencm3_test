
#include <libopencm3/stm32/rcc.h>
#include <libopencm3/stm32/gpio.h>
#include <libopencm3/stm32/i2c.h>
#include <libopencm3/cm3/systick.h>

#include <stdlib.h>

#include "FreeRTOS.h"
#include "task.h"

#include "iic.h"

#define     I2C_REQUEST_WRITE 0
#define     I2C_REQUEST_READ  1

#define     IIC_DELAY_MS 5

/* EEPROM */
#define E24LC32

/* EEPROM 24LC02 */
#ifdef E24LC02
#define EE_I2C_ADDR         (0xA0>>1)
#define EE_PAGE_SIZE        8
#define EE_MEM_ADDR_WIDTH   MEM_ADDR_WIDTH_8BIT
#define EE_SIZE_BYTES       (2048/8)
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
#define EE_PAGE_SIZE        32
#define EE_MEM_ADDR_WIDTH   MEM_ADDR_WIDTH_16BIT
#define EE_SIZE_BYTES       (32768/8)
#endif


static eeprom_t eep_dev;
eeprom_t* eeprom = &eep_dev;


/* RTC DS3107 */

#define RTC_CONVERT_BIN2BCD(__VALUE__) (uint8_t)((((__VALUE__) / 10U) << 4U) | ((__VALUE__) % 10U))
#define RTC_CONVERT_BCD2BIN(__VALUE__) (uint8_t)(((uint8_t)((__VALUE__) & (uint8_t)0xF0U) >> (uint8_t)0x4U) * 10U + ((__VALUE__) & (uint8_t)0x0FU))


#define RTC_I2C_ADDR        0x68

#define RTC_SECONDS_REG     0x00
#define RTC_MINUTES_REG     0x01
#define RTC_HOURS_REG       0x02
#define RTC_WEEKDAY_REG     0x03
#define RTC_DATE_REG        0x04
#define RTC_MONTH_REG       0x05
#define RTC_YEAR_REG        0x06
#define RTC_CONTROL_REG     0x07
#define RTC_INIT_REG        0x08

/* Backup registers: 0x08-0x3F */
#define RTC_BACKUP_REGS_ADDR 0x08
#define RTC_BACKUP_REGS_END  0x3F
#define RTC_BACKUP_REGS_QUANT (RTC_BACKUP_REGS_END - RTC_BACKUP_REGS_ADDR)

#define RTC_CLOCK_HALT_BIT   (1 << 7)
#define RTC_AM_PM_BIT        (1 << 6)
#define RTC_OUT_BIT          (1 << 7)
#define RTC_SQWE_BIT         (1 << 4)
#define RTC_RS1_BIT          (1 << 1)
#define RTC_RS0_BIT          (1 << 0)

#define MON_JANUARY     0x01
#define MON_FABRUARY    0x02
#define MON_MARCH       0x03
#define MON_APRIL       0x04
#define MON_MAY         0x05
#define MON_JUNE        0x06
#define MON_JULY        0x07
#define MON_AUGUST      0x08
#define MON_SEPTEMBER   0x09
#define MON_OCTOBER     0x0A
#define MON_NOVEMBER    0x0B
#define MON_DECEMBER    0x0C

#define WDAY_SUNDAY     0x00
#define WDAY_MONDAY     0x01
#define WDAY_TUESDAY    0x02
#define WDAY_WEDNESDAY  0x03
#define WDAY_THURSDAY   0x04
#define WDAY_FRIDAY     0x05
#define WDAY_SATURDAY   0x06


static rtc_t rtc_dev;
rtc_t* rtc = &rtc_dev;


/* Port Expander MCP23017 */
#define MCP_I2C_ADDR        0x40

#define MCP_IODIRA          0x00
#define MCP_IODIRB          0x01
#define MCP_IPOLA           0x02
#define MCP_IPOLB           0x03
#define MCP_GPINTENA        0x04
#define MCP_GPINTENB        0x05
#define MCP_DEFVALA         0x06
#define MCP_DEFVALB         0x07
#define MCP_INTCONA         0x08
#define MCP_INTCONB         0x09
#define MCP_IOCON          0x0A
//#define MCP_IOCONB          0x0B
#define MCP_GPPUA           0x0C
#define MCP_GPPUB           0x0D
#define MCP_INTFA           0x0E
#define MCP_INTFB           0x0F
#define MCP_INTCAPA         0x10
#define MCP_INTCAPB         0x11
#define MCP_GPIOA           0x12
#define MCP_GPIOB           0x13
#define MCP_OLATA           0x14
#define MCP_OLATB           0x15

#define MCP_PORTA           MCP_GPIOA
#define MCP_PORTB           MCP_GPIOB

#define ALL_INPUTS          0xFF
#define ALL_OUTPUTS         0x00

static mcp_t mcp_dev;
mcp_t* mcp = &mcp_dev;


static void i2c_hw_init(uint32_t i2c)
{
    i2c_reset(i2c);

    rcc_periph_clock_enable(RCC_AFIO);

    switch(i2c){

    default:
        /* Enable clocks for I2C1 */
        rcc_periph_clock_enable(RCC_I2C1);
        rcc_periph_clock_enable(RCC_GPIOB);

        /* Set alternate functions for the SCL and SDA pins of I2C2. */
        gpio_set_mode(GPIOB, GPIO_MODE_OUTPUT_50_MHZ, GPIO_CNF_OUTPUT_ALTFN_OPENDRAIN, GPIO_I2C1_SCL | GPIO_I2C1_SDA);

        break;
    case I2C2:
        /* Enable clocks for I2C2 */
        rcc_periph_clock_enable(RCC_I2C2);
        rcc_periph_clock_enable(RCC_GPIOB);

        /* Set alternate functions for the SCL and SDA pins of I2C2. */
        gpio_set_mode(GPIOB, GPIO_MODE_OUTPUT_50_MHZ, GPIO_CNF_OUTPUT_ALTFN_OPENDRAIN, GPIO_I2C2_SCL | GPIO_I2C2_SDA);

        break;
    }

    i2c_peripheral_disable(i2c);

    i2c_set_speed(i2c, i2c_speed_sm_100k, rcc_apb1_frequency / 1000000);

    i2c_disable_ack(i2c);

    i2c_peripheral_enable(i2c);
}


/*  */
static int iic_check( i2c_dev_t *dev )
{
    uint32_t delay = 0;

    while ((I2C_SR2(dev->handle) & I2C_SR2_BUSY));

    i2c_send_start(dev->handle);
    while (!(I2C_SR1(dev->handle) & I2C_SR1_SB));

    i2c_send_7bit_address(dev->handle, dev->i2c_addr, I2C_WRITE);

    (void)I2C_SR2(dev->handle);

    i2c_send_data(dev->handle, 0x00);
    while (!(I2C_SR1(dev->handle) & (I2C_SR1_BTF))){

#warning systick'as neveikia kaip turi veikti

        if( systick_get_countflag() ){

            delay++;

            if(delay > 10){
                dev->alive = 0;
                return -1;
            }
        }
    }

    dev->alive = 1;

    return 0;
}

/*  */
static int iic_read( i2c_dev_t *dev, uint16_t reg, uint8_t *data, uint16_t len )
{
    i2c_enable_ack(dev->handle);

    while(I2C_SR2(dev->handle) & I2C_SR2_BUSY);

    i2c_send_start(dev->handle);
	while (!(I2C_SR1(dev->handle) & I2C_SR1_SB));

	i2c_send_7bit_address(dev->handle, dev->i2c_addr, I2C_WRITE);
	while (!(I2C_SR1(dev->handle) & I2C_SR1_ADDR));

	(void)I2C_SR2(dev->handle);

    if(dev->mem_addr_width == MEM_ADDR_WIDTH_16BIT){
        i2c_send_data(dev->handle, (uint8_t)((reg>>8)&0x00FF));
        while (!(I2C_SR1(dev->handle) & (I2C_SR1_BTF)));
    }

    i2c_send_data(dev->handle, (uint8_t)reg);
    while (!(I2C_SR1(dev->handle) & (I2C_SR1_BTF)));

    i2c_send_start(dev->handle);
    while (!(I2C_SR1(dev->handle) & I2C_SR1_SB));

    i2c_send_7bit_address(dev->handle, dev->i2c_addr, I2C_READ);
    while (!(I2C_SR1(dev->handle) & I2C_SR1_ADDR));

    (void)I2C_SR2(dev->handle);

    while(len-- > 1)
    {
        while (!(I2C_SR1(dev->handle) & I2C_SR1_RxNE));
        *data++ = i2c_get_data(dev->handle);
    }

    i2c_disable_ack(dev->handle);

    i2c_send_stop(dev->handle);

    while (!(I2C_SR1(dev->handle) & I2C_SR1_RxNE));
    *data = i2c_get_data(dev->handle);

    return 0;
}

/*  */
static int iic_write( i2c_dev_t *dev, uint16_t reg, uint8_t *data, uint16_t len )
{
    i2c_disable_ack(dev->handle);

    while(I2C_SR2(dev->handle) & I2C_SR2_BUSY);

    i2c_send_start(dev->handle);
	while (!(I2C_SR1(dev->handle) & I2C_SR1_SB));

	i2c_send_7bit_address(dev->handle, dev->i2c_addr, I2C_WRITE);
	while (!(I2C_SR1(dev->handle) & I2C_SR1_ADDR));

	(void)I2C_SR2(dev->handle);

    if(dev->mem_addr_width == MEM_ADDR_WIDTH_16BIT){
        i2c_send_data(dev->handle, (uint8_t)((reg>>8)&0x00FF));
        while (!(I2C_SR1(dev->handle) & (I2C_SR1_BTF)));
    }

    i2c_send_data(dev->handle, (uint8_t)reg);
    while (!(I2C_SR1(dev->handle) & (I2C_SR1_BTF)));

    while(len--)
    {
        i2c_send_data(dev->handle, *data++);
        while (!(I2C_SR1(dev->handle) & (I2C_SR1_BTF)));
    }

    while (!(I2C_SR1(dev->handle) & (I2C_SR1_BTF | I2C_SR1_TxE)));

	i2c_send_stop(dev->handle);

    return 0;
}

/*  EEPROM funkcijos  */
static int eep_check( eeprom_t* dev )
{
    //return iic_check( &(dev->i2c_base) );
    dev->i2c_base.alive = 1;

    return 0;
}

int eep_read( eeprom_t* dev, uint16_t reg, uint8_t* data, uint16_t len )
{
    i2c_dev_t* i2c = &(dev->i2c_base);

    if(!i2c->alive) return -1;

    return iic_read( &(dev->i2c_base), reg, data, len );
}

int eep_read_byte( eeprom_t* dev, uint16_t reg, uint8_t* data )
{
    return iic_read( &(dev->i2c_base), reg, data, 1 );
}

int eep_read_word( eeprom_t* dev, uint16_t reg, uint16_t* data )
{
    return iic_read( &(dev->i2c_base), reg, (uint8_t*)data, 2 );
}

int eep_read_dword( eeprom_t* dev, uint16_t reg, uint32_t* data )
{
    return iic_read( &(dev->i2c_base), reg, (uint8_t*)data, 4 );
}

int eep_write( eeprom_t* dev, uint16_t reg, uint8_t *data, uint16_t len )
{
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

int eep_write_byte( eeprom_t* dev, uint16_t reg, uint8_t data )
{
    return eep_write( dev, reg, &data, 1 );
}

int eep_write_word( eeprom_t* dev, uint16_t reg, uint16_t data )
{
    return eep_write( dev, reg, (uint8_t*)&data, 2 );
}

int eep_write_dword( eeprom_t* dev, uint16_t reg, uint32_t data )
{
    return eep_write( dev, reg, (uint8_t*)&data, 4 );
}

int eep_clear( eeprom_t* dev )
{
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

int eep_init( eeprom_t* dev )
{
    dev->i2c_base.handle = I2C1;
    dev->i2c_base.i2c_addr = EE_I2C_ADDR;
    dev->i2c_base.devid = 0;
    dev->i2c_base.mem_addr_width = EE_MEM_ADDR_WIDTH;
    dev->i2c_base.alive = 0;
    dev->size_bytes = EE_SIZE_BYTES;
    dev->page_size = EE_PAGE_SIZE;

    i2c_hw_init(dev->i2c_base.handle);

    return eep_check(dev);
}



/* RTC DS1307 funkcijos  */
static int dsrtc_read_register( rtc_t* dev, uint8_t reg, uint8_t* data ){
    return iic_read( &(dev->i2c_base), reg, data, 1 );
}

static int dsrtc_write_register( rtc_t* dev, uint8_t reg, uint8_t data ){
    return iic_write( &(dev->i2c_base), reg, &data, 1 );
}

static int dsrtc_clear_buffers( rtc_t* dev ){

    uint8_t i = 0;

    do{

        if(i < 8 ) dev->datetime_regs[i] = 0;   // kol i < datetime registru kiekio, isvalom ir tuos registrus
        dev->bkp_regs[i++] = 0;

    }while(i < RTC_BACKUP_REGS_QUANT);

    return 0;
}

int dsrtc_get_datetime( rtc_t* dev ) {

    uint8_t temp = 0;

    (void)dsrtc_read_register( dev, RTC_SECONDS_REG, &temp );
    dev->datetime.Seconds = RTC_CONVERT_BCD2BIN(temp&0x7F);

    (void)dsrtc_read_register( dev, RTC_MINUTES_REG, &temp );
    dev->datetime.Minutes = RTC_CONVERT_BCD2BIN(temp&0x7F);

    (void)dsrtc_read_register( dev, RTC_HOURS_REG, &temp );
    dev->datetime.Hours = RTC_CONVERT_BCD2BIN(temp&0x3F);

    (void)dsrtc_read_register( dev, RTC_WEEKDAY_REG, &temp );
    dev->datetime.WeekDay = RTC_CONVERT_BCD2BIN(temp&0x07);

    (void)dsrtc_read_register( dev, RTC_DATE_REG, &temp );
    dev->datetime.Date = RTC_CONVERT_BCD2BIN(temp&0x3F);

    (void)dsrtc_read_register( dev, RTC_MONTH_REG, &temp );
    dev->datetime.Month = RTC_CONVERT_BCD2BIN(temp&0x1F);

    (void)dsrtc_read_register( dev, RTC_YEAR_REG, &temp );
    dev->datetime.Year = RTC_CONVERT_BCD2BIN(temp);

    return 0;
}

int dsrtc_set_datetime( rtc_t* dev, uint8_t year, uint8_t month, uint8_t date, uint8_t weekday, uint8_t hour, uint8_t minute, uint8_t second ) {

    dev->datetime.Seconds = RTC_CONVERT_BIN2BCD(second);
    dsrtc_write_register( dev, RTC_SECONDS_REG, dev->datetime.Seconds );

    dev->datetime.Minutes = RTC_CONVERT_BIN2BCD(minute);
    dsrtc_write_register( dev, RTC_MINUTES_REG, dev->datetime.Minutes );

    dev->datetime.Hours = RTC_CONVERT_BIN2BCD(hour);
    dsrtc_write_register( dev, RTC_HOURS_REG, dev->datetime.Hours );

    dev->datetime.WeekDay = RTC_CONVERT_BIN2BCD(weekday);
    dsrtc_write_register( dev, RTC_WEEKDAY_REG, dev->datetime.WeekDay );

    dev->datetime.Date = RTC_CONVERT_BIN2BCD(date);
    dsrtc_write_register( dev, RTC_DATE_REG, dev->datetime.Date );

    dev->datetime.Month = RTC_CONVERT_BIN2BCD(month);
    dsrtc_write_register( dev, RTC_MONTH_REG, dev->datetime.Month );

    dev->datetime.Year = RTC_CONVERT_BIN2BCD(year);
    dsrtc_write_register( dev, RTC_YEAR_REG, dev->datetime.Year );

    return 0;
}

void dsrtc_irq_handler(void){
    //rtc.one_sec_flag = gpio_get(RTC_REQ_GPIO_Port, RTC_REQ_Pin);
    //rtc.need_to_update_datetime_flag = 1;
}

int dsrtc_init( rtc_t* dev ){

    uint8_t temp = 0;

    dev->i2c_base.handle = I2C1;
    dev->i2c_base.i2c_addr = RTC_I2C_ADDR;
    dev->i2c_base.devid = 0;
    dev->i2c_base.mem_addr_width = MEM_ADDR_WIDTH_8BIT;
    dev->i2c_base.alive = 0;
    dev->need_to_update_datetime_flag = 0;
    //dev->one_sec_flag = LL_GPIO_IsInputPinSet(RTC_REQ_GPIO_Port, RTC_REQ_Pin);


    (void)dsrtc_clear_buffers( dev );

    if( dsrtc_read_register( dev, RTC_INIT_REG, &temp ) != 0x55 ){

        (void)dsrtc_write_register( dev, RTC_CONTROL_REG, RTC_CLOCK_HALT_BIT );

        (void)dsrtc_set_datetime( dev, 19, MON_JANUARY, 1, WDAY_TUESDAY, 0, 0, 0 );

        (void)dsrtc_write_register( dev, RTC_CONTROL_REG, RTC_SQWE_BIT );

        (void)dsrtc_write_register( dev, RTC_INIT_REG, 0x55 );
    }

    (void)dsrtc_get_datetime( dev );

    return 0;
}





