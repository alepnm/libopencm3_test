
#include "utasks.h"
#include "adc.h"


void task2(void *args)
{
    char sms[] __attribute__((unused)) = "Task 1\r";

	(void)args;

	while(READ_BIT(sys_status.status, SYS_INIT_BIT) == false) taskYIELD();

	for (;;) {

		//SendStringToQueue( PortA->TxQueue, sms);

        adc_start_conversion();

		vTaskDelay(100);
	}
}
