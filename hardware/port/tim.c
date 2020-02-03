
#include "globals.h"
#include "tim.h"

volatile uint32_t systick = 0;


/*  */
void tim2_init(void)
{
	rcc_periph_clock_enable(RCC_TIM2);
	rcc_periph_reset_pulse(RST_TIM2);

	timer_set_mode(TIM2, TIM_CR1_CKD_CK_INT, TIM_CR1_CMS_EDGE, TIM_CR1_DIR_UP);
	timer_set_prescaler(TIM2, 72);
    timer_set_period(TIM2, 986);    // 1ms periodas

	timer_enable_counter(TIM2);

	nvic_enable_irq(NVIC_TIM2_IRQ);
	timer_enable_irq(TIM2, TIM_DIER_UIE);
}

/*  */
void tim2_isr(void)
{
	if( timer_get_flag(TIM2, TIM_DIER_UIE)) {

		timer_clear_flag(TIM2, TIM_DIER_UIE);

		systick++;

		//gpio_toggle(GPIOC, GPIO13);
	}
}
