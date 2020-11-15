#ifndef SECURITE_H
#define SECURITE_H

/**
	* @brief  
  * @note   
	* @param  
  * @retval 
  */
int CalculerDangerChavirement(int gamma);

/**
	* @brief  
  * @note  
	* @param  
  * @retval 
  */
int RecupRouli(void);


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
void ConfSecurite(void);

/**
	* @brief  
  * @note  
	* @param  
  * @retval 
  */
int CalculerDangerNiveauBatterie(int niveau);

#endif
