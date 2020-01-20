
#include "utasks.h"
#include "ds1307.h"


void task3(void *args)
{
    char sms[] __attribute__((unused)) = "Task 3\r";

	(void)args;

	for (;;) {

        SendStringToQueue( PortA->TxQueue, sms);

        //gpio_toggle(LED1.port, LED1.pin);

        (void)ds_get_datetime( ds1307 );

        vTaskDelay(1000);
	}
}


