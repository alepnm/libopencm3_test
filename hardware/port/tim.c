
#include "tim.h"

#define tim1_isr TIM1_IRQHandler
#define tim2_isr TIM2_IRQHandler
#define tim3_isr TIM3_IRQHandler

volatile uint32_t systick = 0;


/*  */
void tim2_init(void)
{
	rcc_periph_clock_enable(RCC_TIM2);
	rcc_periph_reset_pulse(RST_TIM2);

	timer_set_mode(TIM2, TIM_CR1_CKD_CK_INT, TIM_CR1_CMS_EDGE, TIM_CR1_DIR_UP);
	timer_set_prescaler(TIM2, 72);
    timer_set_period(TIM2, 999);    // 1ms periodas

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


/* us taimeris */
void tim3_init(void)
{
	rcc_periph_clock_enable(RCC_TIM3);
	rcc_periph_reset_pulse(RST_TIM3);

	timer_set_mode(TIM3, TIM_CR1_CKD_CK_INT, TIM_CR1_CMS_EDGE, TIM_CR1_DIR_UP);
	timer_set_prescaler(TIM3, 72);
    timer_set_period(TIM3, 99);

	timer_enable_counter(TIM3);
}

