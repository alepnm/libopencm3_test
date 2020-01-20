#ifndef USART_H_INCLUDED
#define USART_H_INCLUDED

#include <libopencm3/stm32/gpio.h>
#include <libopencm3/stm32/usart.h>

#include "FreeRTOS.h"
#include "queue.h"


#define USART1_IRQHandler usart1_irq_handler
#define USART2_IRQHandler usart2_irq_handler
#define USART3_IRQHandler usart3_irq_handler


#define PRIMARY_PORT    0
#define SECONDARY_PORT  1

#define RX_BUFFER_SIZE  256
#define RX_TIMEOUT  100

typedef struct{
    uint32_t        usart;
    uint16_t        port_timer;
    QueueHandle_t   TxQueue;
    char            rx_buffer[RX_BUFFER_SIZE];
    uint8_t         rx_index;
}usart_port_t;

extern usart_port_t* PortA;
extern usart_port_t* PortB;
extern usart_port_t* PortC;



void    usart_init(void);
uint8_t usart_config( uint8_t port, uint32_t baudrate, uint8_t databits, uint32_t parity );
void    usart_port_handler( usart_port_t* port );
void    usart_porttimer_handler( usart_port_t* port );

#endif /* USART_H_INCLUDED */
