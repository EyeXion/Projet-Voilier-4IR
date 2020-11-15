#ifndef ALLURE_H
#define ALLURE_H

/* PERSONNES QUI ONT TRAVAILLE SUR CE MODULE : PAUL */

/**
	* @brief  Configure la girouette pour la détection de l'allure (sens du vent).
  * @note   Configuration des ports PA6 et 7 en alternate input pour récupérer les signaux
						Configuration des deux channels du TIM3 pour le compteur incrementeur 
						Configuration du PA5 avec une interruption pour capter les tours de girouette
	* @param  None
  * @retval None
  */
void ConfAllure(void);

/**
	* @brief  Renvoi la valeur de l'allure actuelle
  * @note   Représentation de la donnée en degrès -> int entre -180 et 180 
						ATTENTION : -1000 renvoyé si girouette non initialisée
	* @param  None
  * @retval L'allure, représenté en degrès par un entier entre -180 et 180
						ATTENTION : -1000 renvoyé si girouette non initialisée
  */
int RecupAllure(void);

/**
	* @brief  Associe un chaine de caractère a une valeur d'allure.
  * @note   Une chaine de caractère est associé a des intervalles de alpha : 
						 [-45, 45]   -> "Vent debout"
						 [45, 55]    -> "Près"  (et intervalle opposé)
						 [55, 65]    -> "Bon plein"  (et intervalle opposé)
						 [65, 80]    -> "Petit largue"  (et intervalle opposé)
						 [80, 100]   -> "Travers"  (et intervalle opposé)
						 [100, 115]  -> "Largue"  (et intervalle opposé)
						 [115, 155]  -> "Grand largue"  (et intervalle opposé)
						 [155, -155] -> "Vent arrière"
						 Autre       -> "ERROR"
	* @param  alpha : un entier représentant l'allure en ° (-180 à 180)
  * @retval Une chaine de caractère représentant l'allure
  */
char * AllureToString(int alpha);

int GirouetteInitialisee(void);

#endif
