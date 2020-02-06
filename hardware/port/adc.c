

#include "adc.h"



uint8_t channel_array[16];


void adc_init(void)
{
	int i;

	rcc_periph_clock_enable(RCC_ADC1);

	/* Make sure the ADC doesn't run during config. */
	adc_power_off(ADC1);

	/* We configure everything for one single conversion. */
	adc_disable_scan_mode(ADC1);

	adc_set_single_conversion_mode(ADC1);

	adc_disable_external_trigger_regular(ADC1);

	adc_set_right_aligned(ADC1);

	/* We want to read the temperature sensor, so we have to enable it. */
	adc_enable_temperature_sensor();

	adc_set_sample_time_on_all_channels(ADC1, ADC_SMPR_SMP_28DOT5CYC);

	adc_power_on(ADC1);

	/* Wait for ADC starting up. */
	for (i = 0; i < 800000; i++)    /* Wait a bit. */
		__asm__("nop");

	adc_reset_calibration(ADC1);

	adc_calibrate(ADC1);
}



/*   */
uint16_t adc_start_conversion(void)
{
	/* Select the channel we want to convert. 16=temperature_sensor. */
	channel_array[0] = ADC_CHANNEL16;

	adc_set_regular_sequence(ADC1, 1, channel_array);

	adc_start_conversion_direct(ADC1);

	while (!adc_eoc(ADC1));

	return ADC_DR(ADC1);
}
