#ifndef USART_H_INCLUDED
#define USART_H_INCLUDED


#include <libopencm3/stm32/rcc.h>
#include <libopencm3/cm3/nvic.h>
#include <libopencm3/stm32/gpio.h>
#include <libopencm3/stm32/usart.h>

#include "FreeRTOS.h"
#include "queue.h"

#include "globals.h"

#define PRIMARY_PORT    0
#define SECONDARY_PORT  1
#define NEXTION_PORT    PRIMARY_PORT
#define MODBUS_PORT     SECONDARY_PORT

#define RX_BUFFER_SIZE  256
#define RX_TIMEOUT  100


enum {BR9600=0, BR19200, BR57600, BR115200};

extern const uint32_t baud[];


typedef struct{

    uint32_t        usart;
    uint8_t         port_id;

    struct _conf{
        uint8_t     enable      :1;
        uint8_t     bdidx       :2;
        uint8_t     databits    :4;
        uint32_t    stopbits;
        uint32_t    parity;
    }config;

    uint16_t        port_timer;
    QueueHandle_t   TxQueue;
    char            rx_buffer[RX_BUFFER_SIZE];
    uint8_t         rx_index;
}usart_port_t;

extern usart_port_t* PortA;
extern usart_port_t* PortB;
extern usart_port_t* PortC;



void    usart_init(void);
uint8_t usart_config( uint8_t port, uint8_t baudrate, uint8_t databits, uint32_t parity );
void    usart_port_handler( usart_port_t* port );
void    usart_porttimer_handler( usart_port_t* port );

#endif /* USART_H_INCLUDED */
