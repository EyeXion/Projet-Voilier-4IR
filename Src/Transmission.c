#include "Transmission.h"

int msTicks = 0;

int drapeauTransmission = 1;

void ConfSysTick(){
	NVIC_EnableIRQ(SysTick_IRQn);
	SysTick_Config(7200000);
}

void SysTick_Handler(void)  {                               /* SysTick interrupt Handler. */
  msTicks++;  
	
	if (msTicks % 30 == 0){
		drapeauTransmission = 1;
	}
	
	if (msTicks % 90 == 0) {
		drapeauRecupSecurite = 1;
		msTicks = 0;
	}
	
}

void ConfTransmission(){
	
	LL_APB2_GRP1_EnableClock(LL_APB2_GRP1_PERIPH_USART1); //On enable la clock pour l'USARt	
	LL_APB2_GRP1_EnableClock(LL_APB2_GRP1_PERIPH_GPIOA); //enable la clock du gpio où est l'USART
	LL_GPIO_SetPinMode(GPIOA,LL_GPIO_PIN_9,LL_GPIO_MODE_ALTERNATE); // Mode fonction alternative PIN USART Tx
	LL_USART_Enable(USART1); //Enable Usart
	LL_USART_EnableDirectionTx(USART1); //Enable direction Tx
	LL_USART_SetParity(USART1,LL_USART_PARITY_NONE); // disable parity bit
	LL_USART_SetStopBitsLength(USART1,LL_USART_STOPBITS_1);
	LL_USART_SetBaudRate(USART1, 72000000,9600); //Set Baud Rate à 9600 (règle de trois avec 1 pour 36 000 0000
	
	LL_GPIO_SetPinMode(GPIOA,LL_GPIO_PIN_11,LL_GPIO_MODE_OUTPUT); 
	LL_GPIO_SetPinOutputType(GPIOA,LL_GPIO_PIN_11,LL_GPIO_OUTPUT_PUSHPULL);
}

void EnvoiRegulier(char * Allure, char * tension){
	
	LL_GPIO_SetOutputPin(GPIOA,LL_GPIO_PIN_11);
	int tailleAllure = sizeof(Allure);
	int tailleTension = 8; //ATTENTION TAILLE
	int index  = 0;
	while(index < tailleAllure){
		if (LL_USART_IsActiveFlag_TXE(USART1)){ //On regarde si le flag de transmission terminée est actif
			LL_USART_TransmitData8(USART1, (uint8_t) Allure[index]); //On envoie le message (8 bits)
			index++;
		}
	} 
	index = 0;
	while(index < tailleTension){
		if (LL_USART_IsActiveFlag_TXE(USART1)){ //On regarde si le flag de transmission terminée est actif
			LL_USART_TransmitData8(USART1, (uint8_t) tension[index]); //On envoie le message (8 bits)
			index++;
		}
	} 
	LL_GPIO_ResetOutputPin(GPIOA,LL_GPIO_PIN_11);
}

void EnvoiExceptionnel(char * msgAlarme){
	
	LL_GPIO_SetOutputPin(GPIOA,LL_GPIO_PIN_11);
	int tailleMessage = sizeof(msgAlarme);
	int index  = 0;
	while(index < tailleMessage){
		if (LL_USART_IsActiveFlag_TXE(USART1)){ //On regarde si le flag de transmission terminée est actif
			LL_USART_TransmitData8(USART1, (uint8_t) msgAlarme[index]); //On envoie le message (8 bits)
			index++;
		}
	} 
	
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

int main(){
	SystemClock_Config();
	ConfSysTick();
	ConfTransmission();
	char * msg1 = "ok ";
	char * msg2 = "coucou ";	
	
	while(1){
		if (drapeauTransmission){
			//EnvoiRegulier(ToString(RecupAllure()),ToString(RecupTension()));
			EnvoiRegulier(msg1, msg2);
			drapeauTransmission = 0;
		}
	}
}
