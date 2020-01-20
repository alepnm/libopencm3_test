
#include <libopencm3/stm32/gpio.h>
#include <libopencm3/stm32/i2c.h>

#include "ds3107.h"

/* RTC DS3107 */
#define RTC_REQ_INPUT_PORT    GPIOB
#define RTC_REQ_INPUT_PIN     GPIO0

#define RTC_I2C_ADDR        (0x68>>1)

typedef enum {
    RTC_SECONDS_REG =   0x00,
    RTC_MINUTES_REG =   0x01,
    RTC_HOURS_REG =     0x02,
    RTC_WEEKDAY_REG =   0x03,
    RTC_DATE_REG =      0x04,
    RTC_MONTH_REG =     0x05,
    RTC_YEAR_REG =      0x06,
    RTC_CONTROL_REG =   0x07,
    RTC_INIT_REG =      0x08
} dsrtc_reg_t;

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


/* RTC DS1307 funkcijos  */
static int ds_check( rtc_t* dev ){

    //return iic_check( &(dev->i2c_base) );
    dev->i2c_base.alive = 1;

    return 0;
}

static int ds_read_register( rtc_t* dev, dsrtc_reg_t reg, uint8_t* data ){

    return iic_read( &(dev->i2c_base), reg, data, 1 );
}

static int ds_write_register( rtc_t* dev, dsrtc_reg_t reg, uint8_t data ){

    return iic_write( &(dev->i2c_base), reg, &data, 1 );
}

static int ds_clear_buffers( rtc_t* dev ){

    uint8_t i = 0;

    do{

        if(i < 8 ) dev->datetime_regs[i] = 0;
        dev->bkp_regs[i++] = 0;

    }while(i < RTC_BACKUP_REGS_QUANT);

    return 0;
}

int ds_get_datetime( rtc_t* dev ) {

    uint8_t temp = 0;

    (void)ds_read_register( dev, RTC_SECONDS_REG, &temp );
    dev->datetime.Seconds = CONVERT_BCD2BIN(temp&0x7F);

    (void)ds_read_register( dev, RTC_MINUTES_REG, &temp );
    dev->datetime.Minutes = CONVERT_BCD2BIN(temp&0x7F);

    (void)ds_read_register( dev, RTC_HOURS_REG, &temp );
    dev->datetime.Hours = CONVERT_BCD2BIN(temp&0x3F);

    (void)ds_read_register( dev, RTC_WEEKDAY_REG, &temp );
    dev->datetime.WeekDay = CONVERT_BCD2BIN(temp&0x07);

    (void)ds_read_register( dev, RTC_DATE_REG, &temp );
    dev->datetime.Date = CONVERT_BCD2BIN(temp&0x3F);

    (void)ds_read_register( dev, RTC_MONTH_REG, &temp );
    dev->datetime.Month = CONVERT_BCD2BIN(temp&0x1F);

    (void)ds_read_register( dev, RTC_YEAR_REG, &temp );
    dev->datetime.Year = CONVERT_BCD2BIN(temp);

    return 0;
}

int ds_set_datetime( rtc_t* dev, uint8_t year, uint8_t month, uint8_t date, uint8_t weekday, uint8_t hour, uint8_t minute, uint8_t second ) {

    dev->datetime.Seconds = CONVERT_BIN2BCD(second);
    ds_write_register( dev, RTC_SECONDS_REG, dev->datetime.Seconds );

    dev->datetime.Minutes = CONVERT_BIN2BCD(minute);
    ds_write_register( dev, RTC_MINUTES_REG, dev->datetime.Minutes );

    dev->datetime.Hours = CONVERT_BIN2BCD(hour);
    ds_write_register( dev, RTC_HOURS_REG, dev->datetime.Hours );

    dev->datetime.WeekDay = CONVERT_BIN2BCD(weekday);
    ds_write_register( dev, RTC_WEEKDAY_REG, dev->datetime.WeekDay );

    dev->datetime.Date = CONVERT_BIN2BCD(date);
    ds_write_register( dev, RTC_DATE_REG, dev->datetime.Date );

    dev->datetime.Month = CONVERT_BIN2BCD(month);
    ds_write_register( dev, RTC_MONTH_REG, dev->datetime.Month );

    dev->datetime.Year = CONVERT_BIN2BCD(year);
    ds_write_register( dev, RTC_YEAR_REG, dev->datetime.Year );

    return 0;
}

void ds_irq_handler(rtc_t* dev){
    dev->one_sec_flag = gpio_get(RTC_REQ_INPUT_PORT, RTC_REQ_INPUT_PIN);
    dev->need_to_update_datetime_flag = 1;
}

int ds_init( rtc_t* dev ){

    uint8_t temp = 0;

    i2c_dev_t* i2c = &(dev->i2c_base);

    gpio_set_mode(RTC_REQ_INPUT_PORT, GPIO_MODE_OUTPUT_2_MHZ, GPIO_CNF_INPUT_FLOAT, RTC_REQ_INPUT_PIN);

    i2c->handle = I2C1;
    i2c->i2c_addr = RTC_I2C_ADDR;
    i2c->mem_addr_width = MEM_ADDR_WIDTH_8BIT;
    i2c->alive = 0;
    dev->need_to_update_datetime_flag = 0;
    dev->one_sec_flag = gpio_get(RTC_REQ_INPUT_PORT, RTC_REQ_INPUT_PIN);

    (void)ds_clear_buffers( dev );

    i2c_hw_init(i2c->handle);

    (void)ds_check( dev );

    if( ds_read_register( dev, RTC_INIT_REG, &temp ) != 0x55 ){

        (void)ds_write_register( dev, RTC_CONTROL_REG, RTC_CLOCK_HALT_BIT );

        (void)ds_set_datetime( dev, 19, MON_JANUARY, 1, WDAY_TUESDAY, 0, 0, 0 );

        (void)ds_write_register( dev, RTC_CONTROL_REG, RTC_SQWE_BIT );

        (void)ds_write_register( dev, RTC_INIT_REG, 0x55 );
    }

    (void)ds_get_datetime( dev );

    return 0;
}

