#include "stm32f103xb.h" 
#include "stm32f1xx_ll_adc.h"
#include "stm32f1xx_ll_gpio.h"
#include "stm32f1xx_ll_bus.h"
#include "stm32f1xx_ll_utils.h"   // utile dans la fonction SystemClock_Config
#include "stm32f1xx_ll_system.h" // utile dans la fonction SystemClock_Config
#include "stm32f1xx_ll_rcc.h" // utile dans la fonction SystemClock_Config
#include "Voile.h"

#define SEUIL45DEGRESSENS1 (1853)

#define SEUIL45DEGRESSENS2 (2457)


// Seuil represente la valeur (en %) en dessous de laquelle le niveau de batterie est consideree faible
#define Seuil (20)

void ConfSecurite(){
	//On est pas s�rs pour ADC 1, voir datasheet page 28
	
	//Faire les horloges 
	LL_APB2_GRP1_EnableClock(LL_APB2_GRP1_PERIPH_GPIOC);
	LL_APB2_GRP1_EnableClock(LL_APB2_GRP1_PERIPH_ADC1);
	
	
	//On configure les PINS
	LL_GPIO_SetPinMode(GPIOC,LL_GPIO_PIN_0,LL_GPIO_MODE_ANALOG);
	LL_GPIO_SetPinMode(GPIOC,LL_GPIO_PIN_2,LL_GPIO_MODE_ANALOG);
	
	
	//ON se met en mode scan enable pour pouvoir � chaque trigger aller chercher sur plusieur voies.
	LL_ADC_SetSequencersScanMode(ADC1,LL_ADC_SEQ_SCAN_ENABLE);
	
	//Permet de faire en sorte d'aller chercher 2 voies (2 rangs) � chaque trigger
	LL_ADC_INJ_SetSequencerLength(ADC1,LL_ADC_INJ_SEQ_SCAN_ENABLE_2RANKS);
	
	//Permet de ne pas avoir d'int�rruption dans le scan 
	LL_ADC_INJ_SetSequencerDiscont(ADC1,LL_ADC_INJ_SEQ_DISCONT_DISABLE);
	
	//A chaque trigger, on va donc avoir voies de rang 1 puis de rang 2 de scann�es
	
	//On met le rang 1 pour la voie 10 (acc�l�ro)
	LL_ADC_INJ_SetSequencerRanks(ADC1,LL_ADC_INJ_RANK_1,LL_ADC_CHANNEL_10);
	
	//On met rang 2 pour la voie 12 (surveillance batterie)
	LL_ADC_INJ_SetSequencerRanks(ADC1,LL_ADC_INJ_RANK_2,LL_ADC_CHANNEL_12);
	
	//Permet d'avoirdes registres ind�pendants pour chaque voie (chaque rangs en fait, au max 4 + 1 pour les regular)
	LL_ADC_INJ_SetTrigAuto(ADC1,LL_ADC_INJ_TRIG_INDEPENDENT);
	
	
	//Sampling time voie 10 et 12, � changer peut-�tre
	LL_ADC_SetChannelSamplingTime(ADC1,LL_ADC_CHANNEL_10,LL_ADC_SAMPLINGTIME_28CYCLES_5);
	
	LL_ADC_SetChannelSamplingTime(ADC1,LL_ADC_CHANNEL_12,LL_ADC_SAMPLINGTIME_28CYCLES_5);

	
	
	LL_ADC_INJ_SetTriggerSource(ADC1,LL_ADC_INJ_TRIG_SOFTWARE); //permet d'enable le trigger de l'adc par le software
	
	
	
	/*LL_ADC_StartCalibration(ADC1);
	
	
	//Tant que calibration par termin�e on enable pas
	while(LL_ADC_IsCalibrationOnGoing(ADC1)){
	}*/
	
		//Activation de l'ADC (???)
	LL_ADC_Enable(ADC1);
}

int RecupNiveauBatterie(void){
		// Lancement de la conversion
	  LL_ADC_INJ_StartConversionSWStart(ADC1);
		// Attente de la fin de conversion
 		while(!LL_ADC_IsActiveFlag_JEOS(ADC1)){}
		// R?cuperation de la valeur apres conversion
 		int NiveauBatterie = LL_ADC_INJ_ReadConversionData12(ADC1,LL_ADC_INJ_RANK_2);
		// Valeur code sur 12 bits : 0 a 4095 ; on veut Niveau de batterie en pourcentage
		return (int)((float)(NiveauBatterie) * 100.0 / 4095.0) ;
}


int CalculerDangerNiveauBatterie(int niveau){
	if ( niveau <= Seuil)
	{
		return 1;
	}
	else
	{
		return 0;
	}
}


int CalculerDangerChavirement(int gamma){
	if ((gamma > SEUIL45DEGRESSENS2) || (gamma < SEUIL45DEGRESSENS1))
	{
		return 1;
	}
	else
	{
		return 0;
	}
}

int RecupRouli(){
			// Lancement de la conversion
	  LL_ADC_INJ_StartConversionSWStart(ADC1);
		// Attente de la fin de conversion
 		while(!LL_ADC_IsActiveFlag_JEOS(ADC1)){}
		// R?cuperation de la valeur apres conversion
 		int NiveauChavirement = LL_ADC_INJ_ReadConversionData12(ADC1,LL_ADC_INJ_RANK_1);
		return NiveauChavirement;
}

