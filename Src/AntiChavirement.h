#ifndef ANTI_CHAVIREMENT_H
#define ANTI_CHAVIREMENT_H

#include "stm32f103xb.h" 
#include "stm32f1xx_ll_adc.h"
#include "stm32f1xx_ll_gpio.h"
#include "stm32f1xx_ll_bus.h"
#include "stm32f1xx_ll_utils.h"   // utile dans la fonction SystemClock_Config
#include "stm32f1xx_ll_system.h" // utile dans la fonction SystemClock_Config
#include "stm32f1xx_ll_rcc.h" // utile dans la fonction SystemClock_Config



/**
	* @brief  
  * @note   
	* @param  
  * @retval 
  */
int CalculerDanger(int gamma);

/**
	* @brief  
  * @note  
	* @param  
  * @retval 
  */
int RecupRouli();

/**
	* @brief  
  * @note   Fonction à lancer avant toute autre. 
	* @param  None
  * @retval None
  */
void ConfAntiChavirement();

#endif
