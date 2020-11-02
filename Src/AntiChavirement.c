#include "AntiChavirement.h"

int CalculerDanger(int gamma){
}

int RecupRouli(){
}

void ConfAntiChavirement(){
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
	
	
	
	LL_ADC_StartCalibration(ADC1);
	
	
	//Tant que calibration par termin�e on enable pas
	while(LL_ADC_IsCalibrationOnGoing(ADC1)){
	}
	
		//Activation de l'ADC (???)
	LL_ADC_Enable(ADC1);
}


void SystemClock_Config(void)
{
  /* Set FLASH latency */
  LL_FLASH_SetLatency(LL_FLASH_LATENCY_2);

  /* Enable HSE oscillator */
	// ********* Commenter la ligne ci-dessous pour MCBSTM32 *****************
	// ********* Conserver la ligne si Nucl�o*********************************
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
int res12;

int main(){
	SystemClock_Config();
	
	ConfAntiChavirement();
	
	while(1){
		LL_ADC_INJ_StartConversionSWStart(ADC1);
		while(!LL_ADC_IsActiveFlag_JEOS(ADC1)){}
		res10 = LL_ADC_INJ_ReadConversionData12(ADC1,LL_ADC_INJ_RANK_1);
		res12 = LL_ADC_INJ_ReadConversionData12(ADC1,LL_ADC_INJ_RANK_2);
	}
}
