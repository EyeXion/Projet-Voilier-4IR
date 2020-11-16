#include "Allure.h"
#include "Gouvernail.h"
//#include "Heure.h"
#include "Transmission.h"
#include "Voile.h"
#include "Securite.h"

#include "stm32f1xx_ll_rcc.h" // utile dans la fonction SystemClock_Config
#include "stm32f1xx_ll_utils.h"   // utile dans la fonction SystemClock_Config
#include "stm32f1xx_ll_system.h" // utile dans la fonction SystemClock_Config


/* PERSONNES QUI ONT TRAVAILLE SUR CE MODULE : PAUL, ELIES, AURIANE, ELIES */


void SystemClock_Config(void);

void ConfSysTick(void){
	SysTick_Config(7200000);
	NVIC_EnableIRQ(SysTick_IRQn);
}

/**
	* @brief configure tous les periphs qu'on va utiliser en appelant les
					 fonctions de config de chaque "module"
  * @note
	* @param None
  * @retval None
  */
void Setup(){
	
	ConfSecurite();
	ConfAllure();
	ConfVoile();
	ConfGouvernail();
	ConfTransmission();
}


/**
	* @brief
  * @note
	* @param
  * @retval
  */
void Envoi3s(){
	EnvoiRegulier(AllureToString(RecupAllure()), TensionVoileToString(RecupTensionVoile()));
}

/**
	* @brief  tache qui va fonctionner en fond qui gere l'orientation
						des voiles, le gouvernail et aussi le systeme anti-chavirement
  * @note  Le temps d'exécution de cette tache a été mesuré à environ 3.14ms, 
					 on peut donc largement la lancer toutes les 100ms en interruption sans risque de trop mobiliser le CPU
	* @param None
  * @retval None
  */
void Task100ms(){
	
	int dangerRoulis = 0; //Variable qui permet de ne pas tendre les voiles si il y a trop de roulis
	
		//Anti-Chavirement
	if(CalculerDangerChavirement(RecupRouli())){//renvoi boolean : int à 0 si faux et 1 si vrai
		dangerRoulis = 1; //Pour pas retendre directement les voiles juste après.
		TendreVoile(90); //si il y a danger on relache les voiles (relacher les voiles = les mettre � 90)
		EnvoiExceptionnel("Risque de chavirement, voiles choquées");//Et on prévient l'utilisateur
	}
	
	//Anti-Chavirement
	if(CalculerDangerNiveauBatterie(RecupNiveauBatterie())){//renvoi boolean : int à 0 si faux et 1 si vrai
		EnvoiExceptionnel("Batterie faible");//Et on prévient l'utilisateur
	}
	
	//Voiles
	if (!dangerRoulis){ //On vérifie qu'il n'y ait pas de roulis avant de tendre les voiles selon l'allure
		TendreVoile(CalculerTension(RecupAllure())); //on tend la voile grace à la tension obtenue a partir du calcul et de l'allure lue
	}

	//Gouvernail
	CommanderMoteur(LireTelecommande()); //On commande le moteur pour aller à la vitesse lue a la télécommande
}

void SysTick_Handler(void)  {                               /* SysTick interrupt Handler. */
	static int compteur = 0;
	compteur++;
	if (compteur == 30) {
		Envoi3s();
		compteur = 0;
	} else {
		Task100ms();
	}
}

int main(){

	SystemClock_Config();

	Setup();
	
	EnvoiExceptionnel("Veuillez faire tourner la girouette SVP.");
	
	while(!GirouetteInitialisee()) {
		EnvoyerCaractere();
	}
	
	ConfSysTick();
	
	while(1){
		EnvoyerCaractere();
	}
}


/**
  * @brief  System Clock Configuration
  *         The system Clock is configured as follow :
  *            System Clock source            = PLL (HSE)
  *            SYSCLK(Hz)                     = 72000000
  *            HCLK(Hz)                       = 72000000
  *            AHB Prescaler                  = 1
  *            APB1 Prescaler                 = 2
  *            APB2 Prescaler                 = 1
  *            HSE Frequency(Hz)              = 8000000
  *            PLLMUL                         = 9
  *            Flash Latency(WS)              = 2
  * @param  None
  * @retval None
  */
void SystemClock_Config(void)
{
  /* Set FLASH latency */
  LL_FLASH_SetLatency(LL_FLASH_LATENCY_2);

  /* Enable HSE oscillator */
	// ********* Commenter la ligne ci-dessous pour MCBSTM32 *****************
	// ********* Conserver la ligne si Nucl�o*********************************
  //LL_RCC_HSE_EnableBypass();
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
  //LL_Init1msTick(72000000); //!! d�commenter que si l'IT est r�cup�r�e

  /* Update CMSIS variable (which can be updated also through SystemCoreClockUpdate function) */
  LL_SetSystemCoreClock(72000000);
}

/* METTRE SYSTEMCLOCKCONFIG */


/* ==============   BOARD SPECIFIC CONFIGURATION CODE END      ============== */

#ifdef  USE_FULL_ASSERT

/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d", file, line) */

  /* Infinite loop */
  while (1)
  {
  }
}
#endif

/**
  * @}
  */

/**
  * @}
  */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/

