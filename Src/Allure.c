#include "maths.h" //Pour la valeur absolue
#include "stm32f1xx_ll_gpio.h" // GPIO
#include "stm32f1xx_ll_bus.h" // RCC
#include "stm32f1xx_ll_tim.h" // TIMER
#include "stm32f1xx_ll_exti.h" // External Interrup

int allure;
int initialized = 0;

/**
	* @brief  Configure la girouette pour la détection de l'allure (sens du vent).
  * @note   Configuration des ports PA6 et 7 en alternate input pour récupérer les signaux
						Configuration des deux channels du TIM3 pour le compteur incrementeur 
						Configuration du PA5 avec une interruption pour capter les tours de girouette
	* @param  None
  * @retval None
  */
void ConfAllure(void) {
  // Configuration des IO
	  // Liberation de la clock du port A
	LL_APB2_GRP1_EnableClock(LL_APB2_GRP1_PERIPH_GPIOA); 
	
	  // Configuration des IO -> pin A6 et A7 en alternate imput, pin A5 en interruption
	  //LL_GPIO_SetPinMode(GPIOA, LL_GPIO_PIN_5, LL_GPIO_MODE_INPUT);
	LL_GPIO_SetPinMode(GPIOA, LL_GPIO_PIN_6, LL_GPIO_MODE_FLOATING);
	LL_GPIO_SetPinMode(GPIOA, LL_GPIO_PIN_7, LL_GPIO_MODE_FLOATING);
	LL_GPIO_AF_SetEXTISource(LL_GPIO_AF_EXTI_PORTA, LL_GPIO_AF_EXTI_LINE5); 
	
	LL_EXTI_InitTypeDef ExtiInitializer;
	LL_EXTI_StructInit(&ExtiInitializer);
	ExtiInitializer.Line_0_31 = LL_EXTI_LINE_5;
	ExtiInitializer.Mode = LL_EXTI_MODE_IT;
	ExtiInitializer.Trigger = LL_EXTI_TRIGGER_RISING;
	ExtiInitializer.LineCommand = ENABLE;
	LL_EXTI_Init(&ExtiInitializer);
	
	NVIC->IP[23] =  0x43;
	NVIC->ISER[0] |= 0x01 <<23;
	
	//Configuration du timer
	  // Libération de la clock du timer
	LL_APB1_GRP1_EnableClock(LL_APB1_GRP1_PERIPH_TIM3);
	
	  // Configuration des canaux du timer -> TIM3 CH1 et 2 en alternate input
	LL_TIM_ENCODER_InitTypeDef Tim3Initializer;
	LL_TIM_ENCODER_StructInit(&Tim3Initializer);
	Tim3Initializer.EncoderMode = LL_TIM_ENCODERMODE_X2_TI1;
	Tim3Initializer.IC1ActiveInput = LL_TIM_ACTIVEINPUT_DIRECTTI;
	Tim3Initializer.IC1Filter = LL_TIM_IC_FILTER_FDIV1 ;
	Tim3Initializer.IC1Polarity = LL_TIM_IC_POLARITY_RISING;
	Tim3Initializer.IC1Prescaler = LL_TIM_ICPSC_DIV1;
	Tim3Initializer.IC2ActiveInput = LL_TIM_ACTIVEINPUT_DIRECTTI;
	Tim3Initializer.IC2Filter = LL_TIM_IC_FILTER_FDIV1;
	Tim3Initializer.IC2Polarity = LL_TIM_IC_POLARITY_RISING;
	Tim3Initializer.IC2Prescaler = LL_TIM_ICPSC_DIV1;
	LL_TIM_ENCODER_Init(TIM3, &Tim3Initializer);
	LL_TIM_SetAutoReload(TIM3, 359);
	LL_TIM_EnableCounter(TIM3);
}

int RecupAllure(void) {
	if (!initialized) {
		return -1000;
	} else {
		return ((int)LL_TIM_GetCounter(TIM3)) - 180;
	}
}

char * ToString(int alpha) {
	int alpha_abs = abs(alpha);
	if (alpha_abs < 45) {
		return "Vent debout";
	} else if (alpha < 55) {
		return "Près";
	} else if (alpha < 65) {
		return "Bon plein";
	} else if (alpha < 80) {
		return "Petit largue";
	} else if (alpha < 100) {
		return "Travers";
	} else if (alpha < 115) {
		return "Largue";
	} else if (alpha < 155) {
		return "Grand largue";
	} else if (alpha < 180) {
		return "Vent arrière";
	} else {
		return "ERROR";
	}		
}

void EXTI9_5_IRQHandler(void) {
	LL_TIM_SetCounter(TIM3, 0);
	initialized = 1;
	LL_EXTI_ClearFlag_0_31(LL_EXTI_LINE_5);
}
