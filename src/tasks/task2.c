
#include "utasks.h"


void task2(void *args)
{
    char sms[] __attribute__((unused)) = "Task 1\r";

	(void)args;

	for (;;) {

		//SendStringToQueue( PortA->TxQueue, sms);

		vTaskDelay(100);
	}
}
