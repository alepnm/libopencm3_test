#ifndef TASKS_H_INCLUDED
#define TASKS_H_INCLUDED

#include "FreeRTOS.h"
#include "queue.h"
#include "task.h"

#include "globals.h"
#include "usart.h"


extern uint8_t sys_init_flag;

extern void SendToQueue( QueueHandle_t que, char* data, uint8_t len );
extern void SendStringToQueue( QueueHandle_t que, char* data );

void task1(void *args);
void task2(void *args);
void task3(void *args);

#endif /* TASKS_H_INCLUDED */
