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

#include <libopencm3/cm3/common.h>
#include <libopencm3/cm3/cortex.h>

/* FreeRTOS includes */
#include "FreeRTOS.h"
#include "task.h"
#include "utasks.h"

/* system includes */
#include "mcuinit.h"
#include "usart.h"


void main_task(void *args)
{
	(void)args;

	for (;;) {

		taskYIELD();
	}
}


int main(void)
{
    MCU_Init();

    xTaskCreate(main_task, "MainTask", 100, NULL, configMAX_PRIORITIES-2, NULL);
	xTaskCreate(task1, "Task1", 100, NULL, configMAX_PRIORITIES-1, NULL);
	xTaskCreate(task2, "Task2", 100, NULL, configMAX_PRIORITIES-1, NULL);
	xTaskCreate(task3, "Task3", 100, NULL, configMAX_PRIORITIES-1, NULL);


	vTaskStartScheduler();
}

/*    */
void vApplicationTickHook( void ){

}


/*    */
void vApplicationStackOverflowHook(xTaskHandle *pxTask,signed portCHAR *pcTaskName)
{
	(void)pxTask;

	(void)pcTaskName;

	for(;;);
}
