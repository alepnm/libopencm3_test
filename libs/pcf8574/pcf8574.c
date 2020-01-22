
#include "pcf8574.h"
#include "pcf8574_lcd.h"


lcd_t lcd;


static int pcf8574_check( pcf_t* dev ){

    //return iic_check( &(dev->i2c_base) );
    dev->i2c_base.alive = 1;

    return 0;
}

/*   */
void pcf8574_init(pcf_t* dev){

    i2c_dev_t* i2c = &(dev->i2c_base);

    i2c->handle = I2C1;
    i2c->i2c_addr = PCF8574_I2C_ADDRESS;
    i2c->mem_addr_width = MEM_ADDR_WIDTH_8BIT;
    i2c->alive = 0;

    dev->devid = 0;
    dev->port_data = 0;

    pcf8574_check(dev);
}


/*  */
void pcf8574_read_port(pcf_t* dev, uint8_t* data){
    iic_simple_rw( &(dev->i2c_base), data, 1, I2C_READ );
}


/*  */
void pcf8574_write_port(pcf_t* dev, uint8_t data){
    iic_simple_rw( &(dev->i2c_base), &data, 1, I2C_WRITE );
}


/*  */
void pcf8574_write_pin(pcf_t* dev, uint8_t pin, uint8_t state){

    uint8_t port;

    iic_simple_rw( &(dev->i2c_base), &port, 1, I2C_READ );

    if(state == 0){
        CLEAR_BIT(port, 1<<pin);
    }else{
        SET_BIT(port, 1<<pin);
    }

    iic_simple_rw( &(dev->i2c_base), &port, 1, I2C_WRITE );

    dev->port_data = port;
}


/* signalo /INT handleris  */
void pcf8574_irq_handler(void){



}

/************************************************************************************************/
/**                              1602 LCD OVER PCF8574                                         **/
/************************************************************************************************/
/*  */
static void lcd_send_internal(lcd_t* dev, uint8_t data, uint8_t flags){

    uint8_t up = data & 0xF0;
    uint8_t lo = (data << 4) & 0xF0;

    uint8_t data_arr[4];

    data_arr[0] = up|flags|BACKLIGHT|PIN_EN;
    data_arr[1] = up|flags|BACKLIGHT;
    data_arr[2] = lo|flags|BACKLIGHT|PIN_EN;
    data_arr[3] = lo|flags|BACKLIGHT;

    iic_simple_rw( &(dev->i2c_base), data_arr, 4, I2C_WRITE );

    vTaskDelay(IIC_DELAY_MS);
}


/*  */
static void lcd_send_cmd(lcd_t* dev, uint8_t cmd){
    lcd_send_internal(dev, cmd, 0);
}


/*   */
static void lcd_send_data(lcd_t* dev, uint8_t data){
    lcd_send_internal(dev, data, PIN_RS);
}


/*   */
void lcd_send_string(lcd_t* dev, const char *str){

    while(*str) {
        lcd_send_data(dev, (uint8_t)(*str));
        str++;
    }
}


/*   */
void lcd_clear(lcd_t* dev){
    lcd_send_cmd(dev, L1602_CMD_CLEAR);
}


/*   */
void lcd_set_position(lcd_t* dev, uint8_t line, uint8_t pos){

    uint8_t cmd = (L1602_CMD_SET_DDRAM | pos);

    if(line != 0) cmd += 0x40;

    lcd_send_cmd(dev, cmd);
}


/*  */
void lcd_init(lcd_t* dev){

    i2c_dev_t* i2c = &(dev->i2c_base);

    i2c->handle = I2C1;
    i2c->i2c_addr = PCF8574_I2C_ADDRESS;
    i2c->mem_addr_width = MEM_ADDR_WIDTH_8BIT;
    i2c->alive = 1;

    //pcf8574_check(dev);

    lcd_send_cmd(dev, L1602_CMD_FUNCTION|L1602_FLAG_2LINE);  // 4-bit mode, 2 lines, 5x7 format (1)

    lcd_send_cmd(dev, L1602_CMD_RETURN_HOME);  // display & cursor home (keep this!) (2)

    vTaskDelay(IIC_DELAY_MS);

    lcd_send_cmd(dev, L1602_CMD_DISPLAY|L1602_FLAG_DISPLAY_ON);  // display on, right shift, underline off, blink off (3)

    lcd_send_cmd(dev, L1602_CMD_CLEAR);  // clear display (optional here) (4)

    vTaskDelay(IIC_DELAY_MS);


    // set address to 0x00
    lcd_send_cmd(dev, (L1602_CMD_SET_DDRAM | 0x00));
    lcd_send_string(dev, " Using 1602 LCD");

    // set address to 0x40
    lcd_send_cmd(dev, (L1602_CMD_SET_DDRAM | 0x40));
    lcd_send_string(dev, " over I2C bus ");
}
