/*
**
**                           Main.c
**
**
**********************************************************************/
/*
   Last committed:     $Revision: 00 $
   Last changed by:    $Author: $
   Last changed date:  $Date:  $
   ID:                 $Id:  $

**********************************************************************/

//#include <libopencm3/cm3/common.h>
#include <libopencm3/cm3/cortex.h>

/* FreeRTOS includes */
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "utasks.h"

/* system includes */
#include "mcuinit.h"
#include "usart.h"
#include "e24lcxx.h"
#include "ds1307.h"

uint8_t eep_data[256];


/* pagrindinis taskas */
void main_task(void *args)
{
	(void)args;


    //eep_clear(eeprom);
    eep_read( eeprom, 0, eep_data, 256 );


	for (;;) {

        if( uxQueueMessagesWaiting(PortA->TxQueue) ) usart_port_handler( PortA );
        if( uxQueueMessagesWaiting(PortB->TxQueue) ) usart_port_handler( PortB );



		taskYIELD();
	}
}


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

/*  */
int main(void)
{
    MCU_Init();

    xTaskCreate(main_task, "MainTask", 100, NULL, configMAX_PRIORITIES-1, NULL);
	xTaskCreate(task1, "Task1", 100, NULL, configMAX_PRIORITIES-1, NULL);
	xTaskCreate(task2, "Task2", 100, NULL, configMAX_PRIORITIES-1, NULL);
	xTaskCreate(task3, "Task3", 100, NULL, configMAX_PRIORITIES-1, NULL);

	vTaskStartScheduler();
}


/*    */
void vApplicationTickHook( void ){

    usart_porttimer_handler( PortA );
    usart_porttimer_handler( PortB );


}


/*    */
void vApplicationStackOverflowHook(xTaskHandle *pxTask,signed portCHAR *pcTaskName)
{
	(void)pxTask;

	(void)pcTaskName;

	for(;;);
}
