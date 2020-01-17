
#include <libopencm3/stm32/rcc.h>
#include <libopencm3/cm3/nvic.h>
#include <libopencm3/stm32/gpio.h>
#include <libopencm3/stm32/usart.h>


#include "usart.h"

static usart_port_t Ports[3];

usart_port_t* PortA = &Ports[0];
usart_port_t* PortB = &Ports[1];
usart_port_t* PortC = &Ports[2];


void usart_init(void)
{
    usart_config( PRIMARY_PORT, 19200, 8, USART_PARITY_NONE );
    PortA->usart = USART1;
    PortA->port_timer = 0;
    PortA->TxQueue = xQueueCreate(256, 1);
    PortA->rx_index = 0;

    usart_config( SECONDARY_PORT, 19200, 8, USART_PARITY_NONE );
    PortB->usart = USART2;
    PortB->port_timer = 0;
    PortB->TxQueue = xQueueCreate(256, 1);
    PortB->rx_index = 0;
}



uint8_t usart_config( uint8_t port, uint32_t baudrate, uint8_t databits, uint32_t parity ) {

    usart_port_t* sp = &Ports[port];

    switch(port) {
    case 2:

        sp->usart = USART2;

        rcc_periph_clock_enable(RCC_GPIOA);
        gpio_set_mode(GPIOA, GPIO_MODE_OUTPUT_50_MHZ, GPIO_CNF_INPUT_FLOAT, GPIO_USART2_RX);
        gpio_set_mode(GPIOA, GPIO_MODE_OUTPUT_50_MHZ, GPIO_CNF_OUTPUT_ALTFN_PUSHPULL, GPIO_USART2_TX);

        rcc_periph_clock_enable(RCC_USART2);

        nvic_enable_irq(NVIC_USART2_IRQ);

        break;
    case 3:

        sp->usart = USART3;

        rcc_periph_clock_enable(RCC_GPIOB);
        gpio_set_mode(GPIOB, GPIO_MODE_OUTPUT_50_MHZ, GPIO_CNF_INPUT_FLOAT, GPIO_USART3_RX);
        gpio_set_mode(GPIOB, GPIO_MODE_OUTPUT_50_MHZ, GPIO_CNF_OUTPUT_ALTFN_PUSHPULL, GPIO_USART3_TX);

        rcc_periph_clock_enable(RCC_USART3);

        nvic_enable_irq(NVIC_USART3_IRQ);

        break;
    default:

        sp->usart = USART1;

        rcc_periph_clock_enable(RCC_GPIOA);
        gpio_set_mode(GPIOA, GPIO_MODE_OUTPUT_50_MHZ, GPIO_CNF_INPUT_FLOAT, GPIO_USART1_RX);
        gpio_set_mode(GPIOA, GPIO_MODE_OUTPUT_50_MHZ, GPIO_CNF_OUTPUT_ALTFN_PUSHPULL, GPIO_USART1_TX);

        rcc_periph_clock_enable(RCC_USART1);

        nvic_enable_irq(NVIC_USART1_IRQ);

        break;
    }

    sp->port_timer = 0;

    usart_disable(sp->usart);

    usart_set_mode(sp->usart, USART_MODE_TX_RX);

    usart_set_baudrate(sp->usart, baudrate);
    usart_set_stopbits(sp->usart, USART_STOPBITS_1);
    usart_set_parity(sp->usart, parity);

    if(parity != USART_PARITY_NONE) databits = 9;

    usart_set_databits(sp->usart, databits);

    usart_set_flow_control(sp->usart, USART_FLOWCONTROL_NONE);

    usart_enable(sp->usart);

    usart_enable_rx_interrupt(sp->usart);
    usart_disable_tx_interrupt(sp->usart);

    return 0;
}


/*  */
void usart_send_it(uint8_t port, char* data){

    usart_port_t* sp = &Ports[port];

    usart_send(sp->usart, *data);
    usart_enable_tx_interrupt(sp->usart);
}


/*  */
void usart_isr_common(uint8_t port)
{
    usart_port_t* sp = &Ports[port];

	/* Check if we were called because of RXNE. */
	if (((USART_CR1(sp->usart) & USART_CR1_RXNEIE) != 0) &&
	    ((USART_SR(sp->usart) & USART_SR_RXNE) != 0)) {

        sp->rx_buffer[sp->rx_index++] = usart_recv(sp->usart);
        sp->port_timer = RX_TIMEOUT;
	}
}


void USART1_IRQHandler( void )
{
    usart_isr_common(PRIMARY_PORT);
}


void USART2_IRQHandler( void )
{
    usart_isr_common(SECONDARY_PORT);
}


void USART3_IRQHandler( void )
{
    usart_isr_common(2);
}
