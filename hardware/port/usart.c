
#include <libopencm3/stm32/rcc.h>
#include <libopencm3/cm3/nvic.h>


#include "usart.h"


#define     usart_disable_tc_interrupt(sp)      (USART_CR1(sp) &= ~USART_CR1_TCIE)
#define     usart_enable_tc_interrupt(sp)       (USART_CR1(sp) |= USART_CR1_TCIE)
#define     usart_clear_tc_interrupt(sp)        (USART_SR(sp) &= ~USART_SR_TC)
#define     usart_is_active_tc_interrupt(sp)    (USART_SR(sp) & USART_SR_TC)

const uint32_t baud[4] = {9600, 19200, 57600, 115200};

static usart_port_t Ports[3];

usart_port_t* PortA = &Ports[0];
usart_port_t* PortB = &Ports[1];
usart_port_t* PortC = &Ports[2];



/*  */
static void usart_hw_init(uint32_t usart)
{
    switch(usart) {
    default:
        rcc_periph_clock_enable(RCC_GPIOA);
        gpio_set_mode(GPIOA, GPIO_MODE_OUTPUT_10_MHZ, GPIO_CNF_INPUT_FLOAT, GPIO_USART1_RX);
        gpio_set_mode(GPIOA, GPIO_MODE_OUTPUT_10_MHZ, GPIO_CNF_OUTPUT_ALTFN_PUSHPULL, GPIO_USART1_TX);

        rcc_periph_clock_enable(RCC_USART1);

        nvic_enable_irq(NVIC_USART1_IRQ);

        break;
    case USART2:
        rcc_periph_clock_enable(RCC_GPIOA);
        gpio_set_mode(GPIOA, GPIO_MODE_OUTPUT_10_MHZ, GPIO_CNF_INPUT_FLOAT, GPIO_USART2_RX);
        gpio_set_mode(GPIOA, GPIO_MODE_OUTPUT_10_MHZ, GPIO_CNF_OUTPUT_ALTFN_PUSHPULL, GPIO_USART2_TX);

        rcc_periph_clock_enable(RCC_USART2);

        nvic_enable_irq(NVIC_USART2_IRQ);

        break;
    case USART3:
        rcc_periph_clock_enable(RCC_GPIOB);
        gpio_set_mode(GPIOB, GPIO_MODE_OUTPUT_10_MHZ, GPIO_CNF_INPUT_FLOAT, GPIO_USART3_RX);
        gpio_set_mode(GPIOB, GPIO_MODE_OUTPUT_10_MHZ, GPIO_CNF_OUTPUT_ALTFN_PUSHPULL, GPIO_USART3_TX);

        rcc_periph_clock_enable(RCC_USART3);

        nvic_enable_irq(NVIC_USART3_IRQ);

        break;
    }
}


/*  */
void usart_init(void)
{
    PortA->usart = USART1;
    PortA->port_id = PRIMARY_PORT;

    PortA->config.enable = STATE_DISABLED;
    PortA->config.bdidx = 1;
    PortA->config.databits = 8;
    PortA->config.stopbits = USART_STOPBITS_1;
    PortA->config.parity = USART_PARITY_NONE;

    PortA->port_timer = 0;
    PortA->TxQueue = xQueueCreate(256, 1);
    PortA->rx_index = 0;

    usart_hw_init(PortA->usart);

    usart_config( PortA->port_id, BR19200, 8, USART_PARITY_NONE );

    PortB->usart = USART2;
    PortB->port_id = SECONDARY_PORT;

    PortB->config.enable = STATE_DISABLED;
    PortB->config.bdidx = 1;
    PortB->config.databits = 8;
    PortB->config.stopbits = USART_STOPBITS_1;
    PortB->config.parity = USART_PARITY_NONE;

    PortB->port_timer = 0;
    PortB->TxQueue = xQueueCreate(256, 1);
    PortB->rx_index = 0;

    usart_hw_init(PortB->usart);

    usart_config( PortB->port_id, BR19200, 8, USART_PARITY_NONE );
}


/*  */
uint8_t usart_config( uint8_t port, uint8_t baudrate, uint8_t databits, uint32_t parity ) {

    usart_port_t* sp = &Ports[port];

    sp->config.bdidx = 1;
    sp->config.databits = 8;
    sp->config.stopbits = USART_STOPBITS_1;
    sp->config.parity = parity;

    sp->config.enable = STATE_DISABLED;
    usart_disable(sp->usart);

    usart_set_mode(sp->usart, USART_MODE_TX_RX);

    usart_set_baudrate(sp->usart, baud[baudrate]);
    usart_set_stopbits(sp->usart, USART_STOPBITS_1);
    usart_set_parity(sp->usart, parity);

    if(parity != USART_PARITY_NONE) databits = 9;

    usart_set_databits(sp->usart, databits);

    usart_set_flow_control(sp->usart, USART_FLOWCONTROL_NONE);

    usart_enable(sp->usart);

    usart_enable_rx_interrupt(sp->usart);
    usart_disable_tx_interrupt(sp->usart);
    usart_disable_tc_interrupt(sp->usart);

    sp->config.enable = STATE_ENABLED;

    return 0;
}


/*  */
void usart_port_handler( usart_port_t* port ){

    char tmp = 0;

    /* siunciam is TxQueue kol eile netuscia */
    while( uxQueueMessagesWaiting(port->TxQueue) ){

        xQueueReceive( port->TxQueue, &tmp, 0 );

        while ((USART_SR(port->usart) & USART_SR_TXE) == 0) portYIELD();
        USART_DR(port->usart) = (tmp & USART_DR_MASK);
    }
}


/*  */
void usart_porttimer_handler( usart_port_t* port ){

    uint16_t i = 0;

    if(port->port_timer){

        port->port_timer--;

        if(port->port_timer == 0){
        /* cia priemem pranesima is USART1*/
            // apdorojam pranesima
            //SendStringToQueue( PortA->TxQueue, port->rx_buffer);
            /* isvalom rx buferi */
            while( i < RX_BUFFER_SIZE ){
                port->rx_buffer[i] = 0;
                i++;
            }

            port->rx_index = 0;
        }
    }
}



void nextion_cmd(void){}



/*  */
void usart_isr_common(uint8_t port)
{
    usart_port_t* sp = &Ports[port];
    char* ptr = &sp->rx_buffer[sp->rx_index];

	/* Check if we were called because of RXNE. */
	if (((USART_CR1(sp->usart) & USART_CR1_RXNEIE) == USART_CR1_RXNEIE) &&
	    ((USART_SR(sp->usart) & USART_SR_RXNE) == USART_SR_RXNE)) {

        *ptr = usart_recv(sp->usart);
        sp->port_timer = RX_TIMEOUT;

        if( *(ptr-2) == 0xFF && *(ptr-1) == 0xFF && *ptr == 0xFF ){

            nextion_cmd();
        }


        sp->rx_index++;
	}

	/* Check if we were called because of TXE. */
	if (((USART_CR1(sp->usart) & USART_CR1_TXEIE) == USART_CR1_TXEIE) &&
	    ((USART_SR(sp->usart) & USART_SR_TXE) == USART_SR_TXE)) {

	}

	/* Check if we were called because of TC. */
	if (((USART_CR1(sp->usart) & USART_CR1_TCIE) == USART_CR1_TCIE) &&
	    ((USART_SR(sp->usart) & USART_SR_TC) == USART_SR_TC)) {


        usart_clear_tc_interrupt(sp->usart);
        usart_disable_tc_interrupt(sp->usart);
	}
}


void usart1_isr( void )
{
    usart_isr_common(PRIMARY_PORT);
}


void usart2_isr( void )
{
    usart_isr_common(SECONDARY_PORT);
}


void usart3_isr( void )
{
    usart_isr_common(2);
}
