#include "stm32f1xx_ll_gpio.h" // GPIO
#include "stm32f1xx_ll_bus.h" // RCC
#include "stm32f1xx_ll_tim.h" // TIMER
#include "maths.h"

#define MAX_PWM (19999)
#define MAX_PWM_OUTPUT (34999)

/**
	* @brief  Configure le récepteur de la télécommande ainsi que le plateau rotatif.
  * @note   Configuration du port PB6 et 7 en alternate input pour récupérer les signaux
						Configuration des deux channels du TIM4 pour la lecture de la PWM 
						Configuration du PA1 en alternate output (commande PWM vitesse) et PA2 en output push pull pour commander le sens
						Configuration du TIM2_CH2 pour la sortie PWM
	* @param  None
  * @retval None
  */
void ConfGouvernail(void) {
	// Configuration des IO
	  // Liberation de la clock du port A et B
	LL_APB2_GRP1_EnableClock(LL_APB2_GRP1_PERIPH_GPIOA); 
	LL_APB2_GRP1_EnableClock(LL_APB2_GRP1_PERIPH_GPIOB);
	
	  // Configuration des IO -> pin B6 en alternate input, pin A1 en alternate output, et, pin A2 en output push pull
	LL_GPIO_SetPinMode(GPIOB, LL_GPIO_PIN_6, LL_GPIO_MODE_FLOATING);
	LL_GPIO_SetPinMode(GPIOA, LL_GPIO_PIN_1, LL_GPIO_MODE_ALTERNATE); 
	LL_GPIO_SetPinOutputType(GPIOA, LL_GPIO_PIN_1, LL_GPIO_OUTPUT_PUSHPULL);
	LL_GPIO_SetPinMode(GPIOA, LL_GPIO_PIN_2, LL_GPIO_MODE_OUTPUT_2MHz); 
	LL_GPIO_SetPinOutputType(GPIOA, LL_GPIO_PIN_2, LL_GPIO_OUTPUT_PUSHPULL);
	
	
	//Configuration des timers
	  // Libération de la clock du timer
	LL_APB1_GRP1_EnableClock(LL_APB1_GRP1_PERIPH_TIM2);
	LL_APB1_GRP1_EnableClock(LL_APB1_GRP1_PERIPH_TIM4);
	
	  // Configuration des canaux du timer -> TIM2 CH2 et PWM output (plages de 20ms)
	LL_TIM_OC_InitTypeDef Tim2Initializer;
	LL_TIM_OC_StructInit(&Tim2Initializer);
	Tim2Initializer.OCMode = LL_TIM_OCMODE_PWM1;
	LL_TIM_OC_Init(TIM2, LL_TIM_CHANNEL_CH2, &Tim2Initializer);
	LL_TIM_SetAutoReload(TIM2, MAX_PWM);
	LL_TIM_SetPrescaler(TIM2, 71);
	LL_TIM_OC_SetCompareCH2(TIM2, 0);
	LL_TIM_EnableCounter(TIM2);
	LL_TIM_CC_EnableChannel(TIM2, LL_TIM_CHANNEL_CH2);
	
	LL_TIM_IC_InitTypeDef Tim4Initializer;
	LL_TIM_IC_StructInit(&Tim4Initializer);
	Tim4Initializer.ICActiveInput = LL_TIM_ACTIVEINPUT_DIRECTTI;
	Tim4Initializer.ICPolarity = LL_TIM_IC_POLARITY_RISING;
	LL_TIM_IC_Init(TIM4, LL_TIM_CHANNEL_CH1, &Tim4Initializer);
	Tim4Initializer.ICActiveInput = LL_TIM_ACTIVEINPUT_INDIRECTTI;
	Tim4Initializer.ICPolarity = LL_TIM_IC_POLARITY_FALLING;
	LL_TIM_IC_Init(TIM4, LL_TIM_CHANNEL_CH2, &Tim4Initializer);
	LL_TIM_SetAutoReload(TIM4, MAX_PWM_OUTPUT);
	LL_TIM_SetPrescaler(TIM4, 71);
	LL_TIM_SetSlaveMode(TIM4, LL_TIM_SLAVEMODE_RESET);
	LL_TIM_SetTriggerInput(TIM4, LL_TIM_TS_TI1FP1);
	LL_TIM_EnableCounter(TIM4);
	LL_TIM_CC_EnableChannel(TIM4, LL_TIM_CHANNEL_CH1);
	LL_TIM_CC_EnableChannel(TIM4, LL_TIM_CHANNEL_CH2);
}

/**
	* @brief  Renvoi la valeur envoyée par la télécommande 
  * @note   Représentation de la donnée en % bidirectionnel -> int entre -100 et 100 (sens codé par le signe, vitesse par la valeur absolue) 
	* @param  None
  * @retval La commande de la télécommande, représenté en % bidirectionnel -> int entre -100 et 100 (sens codé par le signe, vitesse par la valeur absolue) 
  */
int LireTelecommande(void) {
	int lecture = LL_TIM_IC_GetCaptureCH2(TIM4) + 1;
	return ((lecture - 1500) / 5);
}

/**
	* @brief  Commande le moteur en fonction de la commande donnée.
  * @note   Commande le sens et la vitesse
	* @param  Commande en % bidirectionnel -> int entre -100 et 100 (sens codé par le signe, vitesse par la valeur absolue) 
  * @retval None
  */
void CommanderMoteur(int commande) {
	if (commande < 0) {
		LL_GPIO_SetOutputPin(GPIOA, LL_GPIO_PIN_2);
	} else {
		LL_GPIO_ResetOutputPin(GPIOA, LL_GPIO_PIN_2);
	}
	LL_TIM_OC_SetCompareCH2(TIM2, (int)(((float)abs(commande)) * (((float)MAX_PWM) / 100.0)));
}
