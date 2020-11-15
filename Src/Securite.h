#ifndef SECURITE_H
#define SECURITE_H

/* PERSONNES QUI ONT TRAVAILLE SUR CE MODULE : ELIES, AURIANE, ELISE */


  /**
    * @brief   Mets le flag drapeauDangerRouli a 1 si l angle de roulis est mauvais
    * @note Si le voilier prend un angle de roulis sup�rieur � 40� -> MSG de danger + voiles sont imm�diatement rel�ch�es 
    * @param  un entier representant l angle de roulis (entre 0 et 90 degr�s)
    * @retval NONE
    */
int CalculerDangerChavirement(int gamma);


/**
  * @brief  Recuperer l angle de rouli
  * @note   ADC1 -> Channel 10 (Rang 1)
  * @param  None
  * @retval angle de rouli (entre 0 et 90 degr�s)
  */
int RecupRouli(void);


/**
	* @brief  Recuperer le niveau de batterie
  * @note   ADC1 -> Channel 12 (Rang 2)
	* @param  None
  * @retval niveau de batterie en pourcentage (entre 0 et 100)
  */
int RecupNiveauBatterie(void);


/**
	* @brief  Configuration de l'ADC 1 pour l'accelerom�tre et le niveau de batterie
  * @note   Channels 10 et 12. A lancer en 1er
	* @param  None
  * @retval None
  */
void ConfSecurite(void);

/**
	* @brief  Met le flag de dangerBatterie � 1 si batterie faible.
  * @note
	* @param  un entier representant le niveau de batterie en pourcentage
  * @retval 1 si danger, 0 si pas de danger
  */
int CalculerDangerNiveauBatterie(int niveau);

#endif
