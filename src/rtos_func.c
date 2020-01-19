
#include "rtos_func.h"



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

        while(uxQueueSpacesAvailable(que) == 0) portYIELD();

        if( xQueueSend( que, data++, (TickType_t)10 ) != pdPASS )
        {
            /* Failed to post the message, even after 10 ticks. */
        }
    }
}


