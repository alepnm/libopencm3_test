#ifndef USART_H_INCLUDED
#define USART_H_INCLUDED

#include <libopencm3/stm32/gpio.h>
#include <libopencm3/stm32/usart.h>

#include "FreeRTOS.h"
#include "queue.h"

#include "projdefs.h"

#define PRIMARY_PORT    0
#define SECONDARY_PORT  1
#define NEXTION_PORT    PRIMARY_PORT
#define MODBUS_PORT     SECONDARY_PORT

#define RX_BUFFER_SIZE  256
#define RX_TIMEOUT  100


/* Port Baudrate Register Definitions
8 - port enable/disable
5:7 - baudrate


*/
#define CONF_PORT_ENABLE_Pos 7
#define CONF_PORT_ENABLE_Msk (0x01UL<<CONF_PORT_ENABLE_Pos)
#define CONF_PORT_BR_CURRENT_Pos 5
#define CONF_PORT_BR_CURRENT_Msk (0x07UL<<CONF_PORT_BR_CURRENT_Pos)

#define CONF_ENABLE_PORT1()         SET_BIT(PortA->config, CONF_PORT_ENABLE_Msk)
#define CONF_DISABLE_PORT1()        CLEAR_BIT(PortA->config, CONF_PORT_ENABLE_Msk)
#define CONF_ENABLE_PORT2()         SET_BIT(PortB->config, CONF_PORT_ENABLE_Msk)
#define CONF_DISABLE_PORT2()        CLEAR_BIT(PortB->config, CONF_PORT_ENABLE_Msk)

#define CONF_SET_PORT1_BAUDRATE(x)  MODIFY_REG(PortA->config, CONF_PORT_BR_CURRENT_Msk, (x<<CONF_PORT_BR_CURRENT_Pos))
#define CONF_GET_PORT1_BAUDRATE()    ((PortA->config&CONF_PORT_BR_CURRENT_Msk)>>CONF_PORT_BR_CURRENT_Pos)
#define CONF_SET_PORT2_BAUDRATE(x)  MODIFY_REG(PortB->config, CONF_PORT_BR_CURRENT_Msk, (x<<CONF_PORT_BR_CURRENT_Pos))
#define CONF_GET_PORT2_BAUDRATE()    ((PortB->config&CONF_PORT_BR_CURRENT_Msk)>>CONF_PORT_BR_CURRENT_Pos)


enum {BR9600, BR19200, BR57600, BR115200};

extern const uint32_t baud[];


typedef struct{
    uint32_t        usart;
    uint8_t         config;     // porto config registras
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
