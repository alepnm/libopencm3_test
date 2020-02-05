
//#include <libopencmsis/core_cm3.h>
#include <libopencm3/cm3/common.h>
#include <libopencm3/cm3/cortex.h>
#include <libopencm3/cm3/nvic.h>
#include <libopencm3/stm32/timer.h>


/* FreeRTOS includes */
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "utasks.h"

/* system includes */
#include "globals.h"
#include "tim.h"
#include "usart.h"
#include "rtc.h"
#include "calendar.h"


#include "e24lcxx.h"

/* duomenu struktura, kuri sudarys kintamuju aplinka ir kuri bus saugoma EEPROM'e */
static struct _sys_env{
    struct _dt*         pdt;
    struct _conf*       porta_config;
    struct _conf*       portb_config;
}sys_env;


uint8_t     eep_buf[256];



/*  */
static void clock_init(void)
{
    rcc_clock_setup_in_hse_8mhz_out_72mhz();

    rcc_periph_clock_enable(RCC_GPIOA);
    rcc_periph_clock_enable(RCC_GPIOB);
    rcc_periph_clock_enable(RCC_GPIOC);

    rcc_periph_clock_enable(RCC_USART1);
    rcc_periph_clock_disable(RCC_USART2);
    rcc_periph_clock_disable(RCC_USART3);
}

/*  */
static void gpio_init(void)
{
    gpio_primary_remap(0, 0);   // no remap

    /* Enable GPIO clock for leds. */
    rcc_periph_clock_enable(RCC_GPIOC);

    /* Enable led as output */
    gpio_set_mode(GPIOC, GPIO_MODE_OUTPUT_50_MHZ, GPIO_CNF_OUTPUT_PUSHPULL, GPIO13);

    gpio_set(GPIOC, GPIO13);
}


/*  */
void eep_restore_data(void){


}


/*  */
void eep_save_data(void){


}




/* pagrindinis taskas */
void main_task(void *args)
{
	(void)args;

	eep_init( eeprom );
	eep_read( eeprom, 0, eep_buf, 256);

    eep_restore_data();

	SET_BIT(sys_status.status, SYS_INIT_BIT);

	for (;;) {

        if( uxQueueMessagesWaiting(PortA->TxQueue) ) usart_port_handler( PortA );
        if( uxQueueMessagesWaiting(PortB->TxQueue) ) usart_port_handler( PortB );


        if(UpdateDateTimeRequired){

            gpio_toggle(GPIOC, GPIO13);

            cal_time_process();

            UpdateDateTimeRequired = false;
        }

		taskYIELD();
	}
}


/*  */
void SendToQueue( QueueHandle_t que, char* data, uint8_t len ){

    while(len){

        while(uxQueueSpacesAvailable(que) == 0) portYIELD();

        if( xQueueSend( que, data++, (TickType_t)10 ) != pdPASS )
        {
            /* Failed to post the message, even after 10 ticks. */
        }

        len--;
    }
}



/*  */
void SendStringToQueue( QueueHandle_t que, char* data ){

    while(*data){

        while(uxQueueSpacesAvailable(que) == 0) portYIELD();

        if( xQueueSend( que, data++, (TickType_t)10 ) != pdPASS )
        {
            /* Failed to post the message, even after 10 ticks. */
        }
    }
}


/*  */
int main(void)
{
    sys_status.status = 0;

    clock_init();
    tim2_init();
    gpio_init();
    rtc_init();
    usart_init();


    /*  */
    sys_env.pdt = &datetime;
    sys_env.porta_config = &(PortA->config);
    sys_env.portb_config = &(PortB->config);

    xTaskCreate(main_task, "MainTask", 100, NULL, configMAX_PRIORITIES-1, NULL);
	xTaskCreate(task1, "Task1", 100, NULL, configMAX_PRIORITIES-1, NULL);
	xTaskCreate(task2, "Task2", 100, NULL, configMAX_PRIORITIES-1, NULL);
	xTaskCreate(task3, "Task3", 100, NULL, configMAX_PRIORITIES-1, NULL);

	vTaskStartScheduler();
}


/*    */
void vApplicationTickHook( void ){

    usart_porttimer_handler( PortA );
    usart_porttimer_handler( PortB );


}


/*    */
void vApplicationStackOverflowHook(xTaskHandle *pxTask,signed portCHAR *pcTaskName)
{
	(void)pxTask;

	(void)pcTaskName;

	for(;;);
}


void hard_fault_handler(void)
{
	while (1);
}
