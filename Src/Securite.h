#ifndef SECURITE_H
#define SECURITE_H

/* PERSONNES QUI ONT TRAVAILLE SUR CE MODULE : ELIES, AURIANE, ELISE */


  /**
    * @brief   Mets le flag drapeauDangerRouli a 1 si l angle de roulis est mauvais
    * @note Si le voilier prend un angle de roulis supérieur à 40° -> MSG de danger + voiles sont immédiatement relâchées 
    * @param  un entier representant l angle de roulis (entre 0 et 90 degrés)
    * @retval NONE
    */
int CalculerDangerChavirement(int gamma);


/**
  * @brief  Recuperer l angle de rouli
  * @note   ADC1 -> Channel 10 (Rang 1)
  * @param  None
  * @retval angle de rouli (entre 0 et 90 degrès)
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
	* @brief  Configuration de l'ADC 1 pour l'acceleromètre et le niveau de batterie
  * @note   Channels 10 et 12. A lancer en 1er
	* @param  None
  * @retval None
  */
void ConfSecurite(void);

/**
	* @brief  Met le flag de dangerBatterie à 1 si batterie faible.
  * @note
	* @param  un entier representant le niveau de batterie en pourcentage
  * @retval 1 si danger, 0 si pas de danger
  */
int CalculerDangerNiveauBatterie(int niveau);

#endif
