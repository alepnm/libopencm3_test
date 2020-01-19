#ifndef RTOS_FUNC_H_INCLUDED
#define RTOS_FUNC_H_INCLUDED


#include "FreeRTOS.h"
#include "queue.h"

#include "usart.h"



void SendToQueue( QueueHandle_t que, char* data, uint8_t len );
void SendStringToQueue( QueueHandle_t que, char* data );

#endif /* RTOS_FUNC_H_INCLUDED */
