#ifndef ANTI_CHAVIREMENT_H
#define ANTI_CHAVIREMENT_H

#include "stm32f103xb.h" 


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