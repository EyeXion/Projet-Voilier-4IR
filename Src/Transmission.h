#ifndef TRANSMISSION_H
#define TRANSMISSION_H


/* PERSONNES QUI ONT TRAVAILLE SUR CE MODULE : PAUL, ELIES, AURIANE */


/**
  * @brief  Initialiser la structure de transmission avec les chaines de caractères pour l'envoi régulier
  * @note   Flag d'envoi mis à 1 à la fin pour activer la transmission dans EnvoyerCaractere
  * @param  2 chaines de caractères pour l'allure et la tension des voiles
  * @retval None
  */
void EnvoiRegulier(char * Allure, char * tension);

/**
  * @brief  Initialiser la structure de transmission avec les chaines de caractères pour l'envoi excptionnel
  * @note   Flag d'envoi mis à 1 à la fin pour activer la transmission dans EnvoyerCaractere
						Envoi excpetionnel peut être un message d'alerte pour la batterie ou le roulis
  * @param  A chaine de caractère pour indiquer quel danger est mis en avant
  * @retval None
  */
void EnvoiExceptionnel(char * msgAlarme);

/**
  * @brief  Appelée en BG dans le while(1). Si le flag d'envoi est à 1, envoi le prochain caractère contenu dans la 
						structure transmission
  * @note   
  * @param  None
  * @retval None
  */
void EnvoyerCaractere(void);

/**
  * @brief  Configuration de l'USART pour les transmissions
  * @note   None
  * @param  None
  * @retval None
  */
void ConfTransmission(void);

#endif
