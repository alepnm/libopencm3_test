
#include "iic.h"


static int iic_check( i2c_dev_t *dev ) __attribute__((unused));


/*  */
static int iic_check( i2c_dev_t *dev ){

    uint32_t delay = 0;

    while ((I2C_SR2(dev->handle) & I2C_SR2_BUSY)) wait();

    i2c_send_start(dev->handle);
    while (!(I2C_SR1(dev->handle) & I2C_SR1_SB)) wait();

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
static int iic_send_control_byte( i2c_dev_t *dev, uint16_t reg ){

    while(I2C_SR2(dev->handle) & I2C_SR2_BUSY) wait();

    i2c_send_start(dev->handle);
	while (!(I2C_SR1(dev->handle) & I2C_SR1_SB)) wait();

	i2c_send_7bit_address(dev->handle, dev->i2c_addr, I2C_WRITE);
	while (!(I2C_SR1(dev->handle) & I2C_SR1_ADDR)) wait();

	(void)I2C_SR2(dev->handle);

    if(dev->mem_addr_width == MEM_ADDR_WIDTH_16BIT){
        i2c_send_data(dev->handle, (uint8_t)((reg>>8)&0x00FF));
        while (!(I2C_SR1(dev->handle) & (I2C_SR1_BTF))) wait();
    }

    i2c_send_data(dev->handle, (uint8_t)(reg&0x00FF));
    while (!(I2C_SR1(dev->handle) & (I2C_SR1_BTF))) wait();

    return 0;
}

/*  */
int iic_read( i2c_dev_t *dev, uint16_t reg, uint8_t *data, uint16_t len ){

    i2c_enable_ack(dev->handle);

    iic_send_control_byte(dev, reg);

    i2c_send_start(dev->handle);
    while (!(I2C_SR1(dev->handle) & I2C_SR1_SB)) wait();

    i2c_send_7bit_address(dev->handle, dev->i2c_addr, I2C_READ);
    while (!(I2C_SR1(dev->handle) & I2C_SR1_ADDR)) wait();

    (void)I2C_SR2(dev->handle);

    while(len-- > 1)
    {
        while (!(I2C_SR1(dev->handle) & I2C_SR1_RxNE)) wait();
        *data++ = i2c_get_data(dev->handle);
    }

    i2c_disable_ack(dev->handle);

    i2c_send_stop(dev->handle);

    while (!(I2C_SR1(dev->handle) & I2C_SR1_RxNE)) wait();
    *data = i2c_get_data(dev->handle);

    return 0;
}

/*  */
int iic_write( i2c_dev_t *dev, uint16_t reg, uint8_t *data, uint16_t len ){

    i2c_disable_ack(dev->handle);

    iic_send_control_byte(dev, reg);

    while(len--)
    {
        i2c_send_data(dev->handle, *data++);
        while (!(I2C_SR1(dev->handle) & (I2C_SR1_BTF))) wait();
    }

    while (!(I2C_SR1(dev->handle) & (I2C_SR1_BTF | I2C_SR1_TxE))) wait();

	i2c_send_stop(dev->handle);

    return 0;
}

/*  */
void i2c_hw_init(uint32_t i2c){

    static uint8_t i2c_hw_init_flag = 0;

    if(i2c_hw_init_flag) return;    // i2c hw inicializuotas - iseinam

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

    i2c_hw_init_flag = 1;
}
