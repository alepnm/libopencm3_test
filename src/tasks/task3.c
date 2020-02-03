
#include "utasks.h"


void task3(void *args)
{
    char sms[] __attribute__((unused)) = "Task 3\r";

	(void)args;

	while(READ_BIT(sys_status.status, SYS_INIT_BIT) == false) taskYIELD();

	for (;;) {

        SendStringToQueue( PortA->TxQueue, sms);

        //gpio_toggle(GPIOC, GPIO13);

        //(void)ds_get_datetime( ds1307 );

        vTaskDelay(1000);
	}
}


