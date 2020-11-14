#ifndef GOUVERNAIL_H
#define GOUVERNAIL_H

/**
	* @brief  Configure le r�cepteur de la t�l�commande ainsi que le plateau rotatif.
  * @note   Configuration du port PB6 et 7 en alternate input pour r�cup�rer les signaux de la t�l�commande
						Configuration des deux channels du TIM4 pour la lecture de la PWM 
						Configuration du PA1 en alternate output (commande PWM vitesse) et PA2 en output push pull pour commander le sens
						Configuration du TIM2_CH2 pour la sortie PWM
	* @param  None
  * @retval None
  */
void ConfGouvernail(void);

/**
	* @brief  Renvoi la valeur envoy�e par la t�l�commande 
  * @note   Repr�sentation de la donn�e en % bidirectionnel -> int entre -100 et 100 (sens cod� par le signe, vitesse par la valeur absolue) 
	* @param  None
  * @retval La commande de la t�l�commande, repr�sent� en % bidirectionnel -> int entre -100 et 100 (sens cod� par le signe, vitesse par la valeur absolue) 
  */
int LireTelecommande(void);

/**
	* @brief  Commande le moteur en fonction de la commande donn�e.
  * @note   Commande le sens et la vitesse
	* @param  Commande en % bidirectionnel -> int entre -100 et 100 (sens cod� par le signe, vitesse par la valeur absolue) 
  * @retval None
  */
void CommanderMoteur(int commande);

#endif
