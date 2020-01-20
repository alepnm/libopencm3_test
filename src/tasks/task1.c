
#include "utasks.h"


void task1(void *args)
{
    char sms[] __attribute__((unused)) = "Task 2\r";

	(void)args;

	for (;;) {

		//SendStringToQueue( PortA->TxQueue, sms);

		vTaskDelay(500);
	}
}
