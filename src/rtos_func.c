
#include "rtos_func.h"
#include "mcuinit.h"



/*  */
void SendToQueue( QueueHandle_t que, char* data, uint8_t len ){

    while(len){

        while(uxQueueSpacesAvailable(que) == 0) portYIELD();

        if( xQueueSend( que, data++, (TickType_t)10 ) != pdPASS )
        {
            /* Failed to post the message, even after 10 ticks. */
        }

        len--;
    }
}



/*  */
void SendStringToQueue( QueueHandle_t que, char* data ){

    while(*data){

        if(uxQueueSpacesAvailable(que) == 0) portYIELD();

        if( xQueueSend( que, data++, (TickType_t)10 ) != pdPASS )
        {
            /* Failed to post the message, even after 10 ticks. */
        }
    }
}


/*  */
void PortHandler( usart_port_t* port ){

    char tmp;

    /* siunciam is TxQueue kol eile netuscia */
    while( uxQueueMessagesWaiting(port->TxQueue) ){

        xQueueReceive( port->TxQueue, &tmp, 0 );

        while ((USART_SR(port->usart) & USART_SR_TXE) == 0) portYIELD();
        USART_DR(port->usart) = (tmp & USART_DR_MASK);
    }
}


/*  */
void PortTimerHandler( usart_port_t* port ){

    uint16_t i = 0;

    if(port->port_timer){

        port->port_timer--;

        if(port->port_timer == 0){
        /* cia priemem pranesima is USART1*/
            // apdorojam pranesima

            /* isvalom rx buferi */
            while( i < RX_BUFFER_SIZE ){
                port->rx_buffer[i] = 0;
                i++;
            }

            port->rx_index = 0;
        }
    }
}
