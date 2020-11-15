#ifndef VOILE_H
#define VOILE_H

/* PERSONNES QUI ONT TRAVAILLE SUR CE MODULE : ELIES, AURIANE */

/* RESOLUTION PWM SERVOMOTEUR : 10 POSITIONS POSSIBLES*/

 /**
	* @brief  Configuration du Timer en mode PWM output
  * @note   Timer1 en th�rie
	* @param 
  * @retval None
  */
void ConfVoile(void);

 /**
	* @brief  Retourne la valeur de l'angle de la voile actuel en degr�s
  * @note  
	* @param 
  * @retval Un angle compris entre 5 et 90 (en degr�s) (10 valeurs possibles entre dans [5,10,20,30,40,50,60,70,80,90]
  */
int RecupTensionVoile(void);


 /**
	* @brief  Calcule � partir de l'angle alpha (allure), l'angle theta (de la voile) � avoir 
  * @note  Si alpha < 45, on d�tend la voile (theta = 90)
	* @param  Un entier alpha compris entre 0 et 180
  * @retval Un angle theta compris entre 0 et 90 degr�s (en degr�s)
  */
int CalculerTension(int alpha);


 /**
	* @brief  Met � jour le registre CCR1 pour tendre la voile selon theta 
  * @note  10 niveaus possibles pour la r�solution PWM.
	* @param  Un entier theta entre 0 et 90
  * @retval None
  */
void TendreVoile(int theta);


/**
	* @brief  Associe un chaine de caract�re a une valeur d'angle de voile
  * @note   
	* @param  theta : un entier entre 10 valeurs possibles [5,10,20,30,40,50,60,70,80,90]
  * @retval Une chaine de caract�re repr�sentant l'angle de voile
  */
char * TensionVoileToString(int theta);

#endif
