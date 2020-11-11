#include "Allure.h"
#include "AntiChavirement.h"
#include "Batterie.h"
#include "Gouvernail.h"
#include "Heure.h"
#include "Transmission.h"
#include "Voile.h"

#include "stm32f1xx_ll_rcc.h" // utile dans la fonction SystemClock_Config
#include "stm32f1xx_ll_utils.h"   // utile dans la fonction SystemClock_Config
#include "stm32f1xx_ll_system.h" // utile dans la fonction SystemClock_Config



void SystemClock_Config(void);

/**
	* @brief configure tous les periphs qu'on va utiliser en appelant les 
					 fonctions de config de chaque "module"
  * @note   
	* @param None
  * @retval None
  */
void Setup(){
	//ConfAntiChavirement();
	ConfAllure();
	ConfVoile();
	//ConfGouvernail();
	
	
}


//on utilise EnvoiRegulier de Transmission 
//TODO : récuperer les  valeurs des params de EnvoiRegulier
//Interruption toute les 3s via un Timer qui déclenche cette fonction qui permet d'envoyer ce qu'il faut

/**
	* @brief  
  * @note   
	* @param  
  * @retval 
  */
void Envoi3s(){
	
}

/**
	* @brief  tache qui va fonctionner en fond qui gere l'orientation
						des voiles, le gouvernail et aussi le systeme anti-chavirement
  * @note  relacher les voiles <=> tendre les voiles avec un angle à 90° 
	* @param None
  * @retval None
  */
void Background(){
	//Voiles
	int allure = RecupAllure(); //on recupere l'allulre via la girouette
	int tensionVoile = CalculerTension(allure); //grace à l'allure on peut calculer la tension à appliquer sur la voile
	TendreVoile(CalculerTension(RecupAllure())); //on tend la voile grâce à la tension obtenue
	
	//Gouvernail 
	/*int valeurTelecommande = LireTelecommande(); //on lit la valeur renvoyée par la télécommande
	int vitesse = CalculerVitesse(valeurTelecommande);//gràace à la valeur de la telecommande on calcule la vitesse souhaitée
	CommanderMoteur(vitesse);//On commande le moteur pour aller à la vitesse voulue
	
	//Anti-Chavirement
	int rouli = RecupRouli();
	int danger = CalculerDanger(rouli);//renvoi boolean : int à 0 si faux et 1 si vrai
	if(danger){
		TendreVoile(90); //si il y a danger on relache les voiles (relacher les voiles = les mettre à 90)
	}*/
	
}

int main(){
	
	SystemClock_Config();
	
	Setup();
	
	while(1){
		Background();
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
  //LL_Init1msTick(72000000); !! décommenter que si l'IT est récupérée

  /* Update CMSIS variable (which can be updated also through SystemCoreClockUpdate function) */
  LL_SetSystemCoreClock(72000000);
}



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

