
#include "utasks.h"


void task1(void *args)
{
	(void)args;

	for (;;) {

		//gpio_toggle(LED1.port, LED1.pin);

		vTaskDelay(500);
	}
}
