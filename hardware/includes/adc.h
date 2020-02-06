#ifndef ADC_H_INCLUDED
#define ADC_H_INCLUDED

#include <libopencm3/stm32/rcc.h>
#include <libopencm3/stm32/adc.h>
#include <libopencm3/cm3/nvic.h>



void adc_init(void);
uint16_t adc_start_conversion(void);

#endif /* ADC_H_INCLUDED */
