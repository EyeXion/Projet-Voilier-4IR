#ifndef GOUVERNAIL_H
#define GOUVERNAIL_H

/**
	* @brief  Configure le récepteur de la télécommande ainsi que le plateau rotatif.
  * @note   Configuration du port PB6 et 7 en alternate input pour récupérer les signaux de la télécommande
						Configuration des deux channels du TIM4 pour la lecture de la PWM 
						Configuration du PA1 en alternate output (commande PWM vitesse) et PA2 en output push pull pour commander le sens
						Configuration du TIM2_CH2 pour la sortie PWM
	* @param  None
  * @retval None
  */
void ConfGouvernail(void);

/**
	* @brief  Renvoi la valeur envoyée par la télécommande 
  * @note   Représentation de la donnée en % bidirectionnel -> int entre -100 et 100 (sens codé par le signe, vitesse par la valeur absolue) 
	* @param  None
  * @retval La commande de la télécommande, représenté en % bidirectionnel -> int entre -100 et 100 (sens codé par le signe, vitesse par la valeur absolue) 
  */
int LireTelecommande(void);

/**
	* @brief  Commande le moteur en fonction de la commande donnée.
  * @note   Commande le sens et la vitesse
	* @param  Commande en % bidirectionnel -> int entre -100 et 100 (sens codé par le signe, vitesse par la valeur absolue) 
  * @retval None
  */
void CommanderMoteur(int commande);

#endif
