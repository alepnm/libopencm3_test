
#include "utasks.h"


void task1(void *args)
{
    char sms[] __attribute__((unused)) = "Task 2\r";

	(void)args;

	while(READ_BIT(sys_status.status, SYS_INIT_BIT) == false) taskYIELD();

	for (;;) {

		//SendStringToQueue( PortA->TxQueue, sms);

		vTaskDelay(500);
	}
}
