#ifndef VOILE_H
#define VOILE_H

/* PERSONNES QUI ONT TRAVAILLE SUR CE MODULE : ELIES, AURIANE */

/* RESOLUTION PWM SERVOMOTEUR : 10 POSITIONS POSSIBLES*/

 /**
	* @brief  Configuration du Timer en mode PWM output
  * @note   Timer1 en thérie
	* @param 
  * @retval None
  */
void ConfVoile(void);

 /**
	* @brief  Retourne la valeur de l'angle de la voile actuel en degrés
  * @note  
	* @param 
  * @retval Un angle compris entre 5 et 90 (en degrés) (10 valeurs possibles entre dans [5,10,20,30,40,50,60,70,80,90]
  */
int RecupTensionVoile(void);


 /**
	* @brief  Calcule à partir de l'angle alpha (allure), l'angle theta (de la voile) à avoir 
  * @note  Si alpha < 45, on détend la voile (theta = 90)
	* @param  Un entier alpha compris entre 0 et 180
  * @retval Un angle theta compris entre 0 et 90 degrés (en degrés)
  */
int CalculerTension(int alpha);


 /**
	* @brief  Met à jour le registre CCR1 pour tendre la voile selon theta 
  * @note  10 niveaus possibles pour la résolution PWM.
	* @param  Un entier theta entre 0 et 90
  * @retval None
  */
void TendreVoile(int theta);


/**
	* @brief  Associe un chaine de caractère a une valeur d'angle de voile
  * @note   
	* @param  theta : un entier entre 10 valeurs possibles [5,10,20,30,40,50,60,70,80,90]
  * @retval Une chaine de caractère représentant l'angle de voile
  */
char * TensionVoileToString(int theta);

#endif
