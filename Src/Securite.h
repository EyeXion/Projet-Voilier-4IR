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
void CalculerDanger(int gamma);

/**
	* @brief  
  * @note  
	* @param  
  * @retval 
  */
int RecupRouli();


/**
	* @brief  
  * @note  
	* @param  
  * @retval 
  */
int RecupNiveauBatterie(void);

/**
	* @brief  
  * @note  A lancer en 1er avant tout
	* @param  
  * @retval 
  */
void confSecurite(void);

/**
	* @brief  
  * @note  
	* @param  
  * @retval 
  */
void CalculDangerNiveauBatterie(int niveau );

#endif
