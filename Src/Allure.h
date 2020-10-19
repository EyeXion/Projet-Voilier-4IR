#ifndef ALLURE_H
#define ALLURE_H

/**
	* @brief  Configure la girouette pour la d�tection de l'allure (sens du vent).
  * @note   Configuration des ports PA6 et 7 en alternate input pour r�cup�rer les signaux
						Configuration des deux channels du TIM3 pour le compteur incrementeur 
						Configuration du PA5 avec une interruption pour capter les tours de girouette
	* @param  None
  * @retval None
  */
void ConfAllure();

/**
	* @brief  Renvoi la valeur de l'allure actuelle
  * @note   Repr�sentation de la donn�e en degr�s -> int entre -180 et 180 
	* @param  None
  * @retval L'allure, repr�sent� en degr�s par un entier entre -180 et 180
  */
int RecupAllure();

/**
	* @brief  Associe un chaine de caract�re a une valeur d'allure.
  * @note   Une chaine de caract�re est associ� a des intervalles de alpha : 
						 [-45, 45]   -> "Vent debout"
						 [45, 55]    -> "Pr�s"  (et intervalle oppos�)
						 [55, 65]    -> "Bon plein"  (et intervalle oppos�)
						 [65, 80]    -> "Petit largue"  (et intervalle oppos�)
						 [80, 100]   -> "Travers"  (et intervalle oppos�)
						 [100, 115]  -> "Largue"  (et intervalle oppos�)
						 [115, 155]  -> "Grand largue"  (et intervalle oppos�)
						 [155, -155] -> "Vent arri�re"
						 Autre       -> "ERROR"
	* @param  alpha : un entier repr�sentant l'allure en � (-180 � 180)
  * @retval Une chaine de caract�re repr�sentant l'allure
  */
char * ToString(int alpha);

#endif
