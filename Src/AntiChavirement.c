#include "AntiChavirement.h"

int CalculerDanger(int gamma){
}

int RecupRouli(){
}

void ConfAntiChavirement(){
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
