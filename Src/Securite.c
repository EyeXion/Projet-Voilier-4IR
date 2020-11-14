#include "Securite.h"


// Seuil represente la valeur (en %) en dessous de laquelle le niveau de batterie est consideree faible
#define Seuil (20)

// Seuil represente la valeur de l'angle au dessus de laquelle le chavirement est trop prononcé
#define SeuilAngle (40)


int drapeauRecupSecurite = 0; // Flag qui est mis à 1 toute les 10 sec pour recuperer niveau batterie/rouli et envoi régulier
int drapeauDangerBatterie = 0; //Flag qui est mis à 1 toute les 10 sec si batterie faible --> envoie message alarme
int drapeauDangerRouli = 0; //Flag qui est mis à 1 toute les 10 sec si rouli pas bon --> envoie message alarme


void ConfSecurite(){
	//On est pas sûrs pour ADC 1, voir datasheet page 28
	
	//Faire les horloges 
	LL_APB2_GRP1_EnableClock(LL_APB2_GRP1_PERIPH_GPIOC);
	LL_APB2_GRP1_EnableClock(LL_APB2_GRP1_PERIPH_ADC1);
	
	
	//On configure les PINS
	LL_GPIO_SetPinMode(GPIOC,LL_GPIO_PIN_0,LL_GPIO_MODE_ANALOG);
	LL_GPIO_SetPinMode(GPIOC,LL_GPIO_PIN_2,LL_GPIO_MODE_ANALOG);
	
	
	//ON se met en mode scan enable pour pouvoir à chaque trigger aller chercher sur plusieur voies.
	LL_ADC_SetSequencersScanMode(ADC1,LL_ADC_SEQ_SCAN_ENABLE);
	
	//Permet de faire en sorte d'aller chercher 2 voies (2 rangs) à chaque trigger
	LL_ADC_INJ_SetSequencerLength(ADC1,LL_ADC_INJ_SEQ_SCAN_ENABLE_2RANKS);
	
	//Permet de ne pas avoir d'intérruption dans le scan 
	LL_ADC_INJ_SetSequencerDiscont(ADC1,LL_ADC_INJ_SEQ_DISCONT_DISABLE);
	
	//A chaque trigger, on va donc avoir voies de rang 1 puis de rang 2 de scannées
	
	//On met le rang 1 pour la voie 10 (accéléro)
	LL_ADC_INJ_SetSequencerRanks(ADC1,LL_ADC_INJ_RANK_1,LL_ADC_CHANNEL_10);
	
	//On met rang 2 pour la voie 12 (surveillance batterie)
	LL_ADC_INJ_SetSequencerRanks(ADC1,LL_ADC_INJ_RANK_2,LL_ADC_CHANNEL_12);
	
	//Permet d'avoirdes registres indépendants pour chaque voie (chaque rangs en fait, au max 4 + 1 pour les regular)
	LL_ADC_INJ_SetTrigAuto(ADC1,LL_ADC_INJ_TRIG_INDEPENDENT);
	
	
	//Sampling time voie 10 et 12, à changer peut-être
	LL_ADC_SetChannelSamplingTime(ADC1,LL_ADC_CHANNEL_10,LL_ADC_SAMPLINGTIME_28CYCLES_5);
	
	LL_ADC_SetChannelSamplingTime(ADC1,LL_ADC_CHANNEL_12,LL_ADC_SAMPLINGTIME_28CYCLES_5);

	
	
	LL_ADC_INJ_SetTriggerSource(ADC1,LL_ADC_INJ_TRIG_SOFTWARE); //permet d'enable le trigger de l'adc par le software
	
	
	
	LL_ADC_StartCalibration(ADC1);
	
	
	//Tant que calibration par terminée on enable pas
	while(LL_ADC_IsCalibrationOnGoing(ADC1)){
	}
	
		//Activation de l'ADC (???)
	LL_ADC_Enable(ADC1);
}

/**
	* @brief  Recuperer le niveau de batterie
  * @note   ADC1 -> Channel 12 (Rang 2)
	* @param  None
  * @retval niveau de batterie en pourcentage
  */
int RecupNiveauBatterie(void){
		// Lancement de la conversion
	  LL_ADC_INJ_StartConversionSWStart(ADC1);
		// Attente de la fin de conversion
 		while(!LL_ADC_IsActiveFlag_JEOS(ADC1)){}
		// R?cuperation de la valeur apres conversion
 		int NiveauBatterie = LL_ADC_INJ_ReadConversionData12(ADC1,LL_ADC_INJ_RANK_2);
		// Valeur code sur 12 bits : 0 a 4095 ; on veut Niveau de batterie en pourcentage
		return NiveauBatterie * 100 / 4095 ;
}


/**
	* @brief  Met le flag de dangerBatterie à 1 si batterie faible.
  * @note
	* @param  un entier representant le niveau de batterie en pourcentage
  * @retval 
  */
void CalculDangerNiveauBatterie(int niveau){
	if ( niveau <= Seuil)
	{
		drapeauDangerBatterie = 1;
	}
	else
	{
		drapeauDangerBatterie = 0 ;
	}
}


  /*
    * @brief   Mets le flag drapeauDangerRouli a 1 si l angle de rouli est mauvais
    * @note Si le voilier prend un angle de roulis supe´rieur a` 40° -> MSG de danger + voiles sont imme´diatement rela^che´es 
      * @param  un entier representant l angle de rouli
    * @retval NONE
    */
void CalculerDangerChavirement(int gamma){
	if ( gamma >= SeuilAngle)
	{
		drapeauDangerRouli = 1;
		TendreVoile(90); //On lache les voiles si danger de chavirement
	}
	else
	{
		drapeauDangerRouli = 0 ;
	}
}

/*
  * @brief  Recuperer l angle de rouli
  * @note   ADC1 -> Channel 10 (Rang 1)
  * @param  None
  * @retval angle de rouli
  */
int RecupRouli(){
			// Lancement de la conversion
	  LL_ADC_INJ_StartConversionSWStart(ADC1);
		// Attente de la fin de conversion
 		while(!LL_ADC_IsActiveFlag_JEOS(ADC1)){}
		// R?cuperation de la valeur apres conversion
 		int NiveauChavirement = LL_ADC_INJ_ReadConversionData12(ADC1,LL_ADC_INJ_RANK_1);
		return 0; // Trouver le calcul de l'angle
}

void SystemClock_Config(void)
{
  /* Set FLASH latency */
  LL_FLASH_SetLatency(LL_FLASH_LATENCY_2);

  /* Enable HSE oscillator */
	// ********* Commenter la ligne ci-dessous pour MCBSTM32 *****************
	// ********* Conserver la ligne si Nucléo*********************************
  LL_RCC_HSE_EnableBypass();
  LL_RCC_HSE_Enable();
  while(LL_RCC_HSE_IsReady() != 1)
  {
  };

  /* Main PLL configuration and activation */
  LL_RCC_PLL_ConfigDomain_SYS(LL_RCC_PLLSOURCE_HSE_DIV_1, LL_RCC_PLL_MUL_9);

  LL_RCC_PLL_Enable();
  while(LL_RCC_PLL_IsReady() != 1)
  {
  };

  /* Sysclk activation on the main PLL */
  LL_RCC_SetAHBPrescaler(LL_RCC_SYSCLK_DIV_1);
  LL_RCC_SetSysClkSource(LL_RCC_SYS_CLKSOURCE_PLL);
  while(LL_RCC_GetSysClkSource() != LL_RCC_SYS_CLKSOURCE_STATUS_PLL)
  {
  };

  /* Set APB1 & APB2 prescaler*/
  LL_RCC_SetAPB1Prescaler(LL_RCC_APB1_DIV_2);
  LL_RCC_SetAPB2Prescaler(LL_RCC_APB2_DIV_1);

  /* Set systick to 1ms in using frequency set to 72MHz */
  LL_Init1msTick(72000000); // utile lorsqu'on utilise la fonction LL_mDelay

  /* Update CMSIS variable (which can be updated also through SystemCoreClockUpdate function) */
  LL_SetSystemCoreClock(72000000);
}

int res10;
int level ;
int danger;
int rouli;

//Dans le main voilier.c en theorie
int main(){
	SystemClock_Config();
	ConfSecurite();
	
	
	while(1){
		
		/* Envoi message toute les 3 sec sur l'Allure et la tension voile */
		if (drapeauTransimission){ 
			//Envoi message régulier
			drapeauTransmission = 0;
		}
		
		/* Recuperation toute les 10 sec niveau batterie et angle chavirement, puis calcul des dangers */
		if (drapeauRecupSecurite){
			level = RecupNiveauBatterie();
			CalculDangerNiveauBatterie( level );
			rouli = RecupRouli()
			CalculerDanger(rouli);
			drapeauRecupSecurite = 0;
		}
		
		/*Si batterie faible après recup (toute les 10 sec), envoi alerte batterie faible */
		if (drapeauDangerBatterie){
			char * msgBatterie = "Batterie faible";
			EnvoiExcpetionnel(msgBatterie);
			drapeauDangerBatterie = 0;
		}
		
		/*Si angle chavirement pas bon après recup (toute les 10 sec), envoi alerte chavirement */
		if (drapeauDangerRouli){
			char * msgRouli = "Bateau chavire !";
			EnvoiExcpetionnel(msgRouli);
			drapeauDangerRouli = 0;
		}
		
	}
}
