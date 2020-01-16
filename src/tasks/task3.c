
#include "utasks.h"


void task3(void *args)
{
	(void)args;

	for (;;) {

		gpio_toggle(LED1.port, LED1.pin);

		//usart_send(USART1, '0');
		//usart_enable_tx_interrupt(USART1);

		vTaskDelay(3000);
	}
}
