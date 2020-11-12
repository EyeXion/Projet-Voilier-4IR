#include "stm32f1xx_ll_gpio.h" // GPIO
#include "stm32f1xx_ll_bus.h" // RCC
#include "stm32f1xx_ll_tim.h" // TIMER

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
	
	  // Configuration des IO -> pin B6 et 7 en alternate input, pin A1 en alternate output, et, pin A2 en output push pull
	LL_GPIO_SetPinMode(GPIOB, LL_GPIO_PIN_6, LL_GPIO_MODE_FLOATING);
	LL_GPIO_SetPinMode(GPIOB, LL_GPIO_PIN_7, LL_GPIO_MODE_FLOATING);
	LL_GPIO_SetPinMode(GPIOA, LL_GPIO_PIN_1, LL_GPIO_MODE_ALTERNATE); 
	LL_GPIO_SetPinOutputType(GPIOA, LL_GPIO_PIN_1, LL_GPIO_OUTPUT_PUSHPULL);
	LL_GPIO_SetPinMode(GPIOA, LL_GPIO_PIN_2, LL_GPIO_MODE_OUTPUT); 
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
	LL_TIM_SetAutoReload(TIM2, 19999);
	LL_TIM_SetPrescaler(TIM2, 71);
	LL_TIM_OC_SetCompareCH2(TIM2, 0);
	LL_TIM_EnableCounter(TIM2);
	LL_TIM_CC_EnableChannel(TIM2, LL_TIM_CHANNEL_CH2);
}

/**
	* @brief  Renvoi la valeur envoyée par la télécommande 
  * @note   Représentation de la donnée en % bidirectionnel -> int entre -100 et 100 (sens codé par le signe, vitesse par la valeur absolue) 
	* @param  None
  * @retval La commande de la télécommande, représenté en % bidirectionnel -> int entre -100 et 100 (sens codé par le signe, vitesse par la valeur absolue) 
  */
int LireTelecommande(void) {
	return 0;
}

/**
	* @brief  Commande le moteur en fonction de la commande donnée.
  * @note   Commande le sens et la vitesse
	* @param  Commande en % bidirectionnel -> int entre -100 et 100 (sens codé par le signe, vitesse par la valeur absolue) 
  * @retval None
  */
void CommanderMoteur(int commande) {
}
