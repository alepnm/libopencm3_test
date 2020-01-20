
#include "utasks.h"


void task3(void *args)
{
    char sms[] __attribute__((unused)) = "Task 3\r";

	(void)args;

	for (;;) {

        SendStringToQueue( PortA->TxQueue, sms);

        //gpio_toggle(LED1.port, LED1.pin);

        vTaskDelay(1000);
	}
}


