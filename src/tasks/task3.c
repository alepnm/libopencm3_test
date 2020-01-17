
#include "utasks.h"


void task3(void *args)
{
    char sms[] = "Task 3\r";

	(void)args;

	for (;;) {

        SendStringToQueue( PortA->TxQueue, sms);


        vTaskDelay(1000);
	}
}


